#pragma once

#include <drogon/drogon.h>
#include <drogon/HttpController.h>
#include <trantor/utils/Date.h>

#include <cstdlib>
#include <fstream>
#include <string>

using namespace drogon;
using namespace drogon::orm;

class Extrato : public drogon::HttpController<Extrato>
{
  public:
    virtual void extrato(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback,
        std::string id);
    virtual void transacoes(
        const HttpRequestPtr &req,
        std::function<void(const HttpResponsePtr &)> &&callback,
        std::string id);
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(Extrato::extrato, "/clientes/{}/extrato", Get);
    ADD_METHOD_TO(Extrato::transacoes, "/clientes/{}/transacoes", Post);
    METHOD_LIST_END
};
