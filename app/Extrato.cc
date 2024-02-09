#include "Extrato.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <stdexcept>

using namespace std;
using namespace drogon;
using namespace drogon::orm;

void Extrato::extrato(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    std::string id)
{
    Json::Value json;
    Json::Value& transacoes = json["ultimas_transacoes"];

    auto clientPtr = drogon::app().getDbClient("rinha");
    *clientPtr << "SELECT HIGH_PRIORITY saldo, limite FROM clientes WHERE id=?"
            << id << Mode::Blocking
            >> [&callback, &json](const drogon::orm::Result &result)
                {
                    if (result.size() == 0)
                    {
                        Json::Value err;
                        auto resp = HttpResponse::newHttpJsonResponse(err);
                        resp->setStatusCode(k404NotFound);
                        callback(resp);
                        return;
                    }

                    const trantor::Date now = trantor::Date::date();

                    Json::Value saldo;
                    saldo["total"] = result[0]["saldo"].as<int>();
                    saldo["limite"] = result[0]["limite"].as<int>();
                    saldo["data_extrato"] = now.toCustomedFormattedString("%Y-%m-%d %H:%M:%S", true);
                    json["saldo"] = saldo;
                }
            >> [](const DrogonDbException &e)
                {
                    std::cerr << "error:" << e.base().what() << std::endl;
                };

    *clientPtr << "SELECT valor, tipo, descricao, realizada_em FROM transacoes WHERE cliente_id=? ORDER BY id DESC LIMIT 10"
            << id << Mode::Blocking
            >> [&callback, &transacoes, &json](const drogon::orm::Result &result)
                {
                    unsigned int i = 0;
                    for (auto const &row : result)
                    {
                        transacoes[i]["valor"] = row["valor"].as<int>();
                        transacoes[i]["tipo"] = row["tipo"].as<std::string>();
                        transacoes[i]["descricao"] = row["descricao"].as<std::string>();
                        transacoes[i]["realizada_em"] = row["realizada_em"].as<std::string>();
                        i++;
                    }

                    callback(HttpResponse::newHttpJsonResponse(json));
                }
            >> [](const DrogonDbException &e)
                {
                    std::cerr << "error:" << e.base().what() << std::endl;
                };
}

void Extrato::transacoes(
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback,
    std::string id)
{
    auto json = req->getJsonObject();
    if (!json)
    {
        Json::Value err;
        auto resp = HttpResponse::newHttpJsonResponse(err);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

    std::string typ = (*json)["tipo"].asString();

    if ((typ != "c" && typ != "d") || (*json)["valor"].asString().find(".") != string::npos || (*json)["descricao"].asString().size() > 10 || (*json)["descricao"].asString().size() < 1 || (*json)["descricao"].asString() == "null")
    {
        Json::Value err0;
        auto resp = HttpResponse::newHttpJsonResponse(err0);
        resp->setStatusCode(k422UnprocessableEntity);
        callback(resp);
        return;
    }

    int32_t valor = (*json)["valor"].as<int32_t>();
    auto clientPtr = drogon::app().getDbClient("rinha");
    auto transPtr = clientPtr->newTransaction();
    *transPtr << "SELECT HIGH_PRIORITY saldo, limite FROM clientes WHERE id=? LOCK IN SHARE MODE"
            << id << Mode::Blocking
            >> [&transPtr, &callback, &id, &typ, &valor](const drogon::orm::Result &result)
                {
                    if (result.size() == 0)
                    {
                        Json::Value err2;
                        auto resp = HttpResponse::newHttpJsonResponse(err2);
                        resp->setStatusCode(k404NotFound);
                        callback(resp);
                        return;
                    }

                    int32_t saldo = result[0]["saldo"].as<int32_t>();
                    if (typ == "c")
                    {
                        saldo += valor;
                    }
                    else
                    {
                        saldo -= valor;
                        if (saldo <= (result[0]["limite"].as<std::int32_t>() * -1))
                        {
                            Json::Value err3;
                            auto resp = HttpResponse::newHttpJsonResponse(err3);
                            resp->setStatusCode(k422UnprocessableEntity);
                            callback(resp);
                            transPtr->rollback();
                            return;
                        }

                    }

                    Json::Value payload;
                    payload["saldo"] = saldo;
                    payload["limite"] = result[0]["limite"].as<int>();
                    callback(HttpResponse::newHttpJsonResponse(payload));

                    *transPtr << "UPDATE clientes SET saldo=? WHERE id=?"
                                              << saldo
                                              << id;
                }
            >> [](const DrogonDbException &e)
                {
                    std::cerr << "error:" << e.base().what() << std::endl;
                };

    *clientPtr  << "INSERT LOW_PRIORITY INTO transacoes (cliente_id, tipo, valor, descricao) VALUES (?,?,?,?)"
            << id
            << typ
            << valor
            << (*json)["descricao"].asString();
}

