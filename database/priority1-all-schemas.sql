-- 
-- RINHA
--

CREATE DATABASE IF NOT EXISTS `rinha` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE rinha;

CREATE TABLE clientes (
  id INT(1) AUTO_INCREMENT,
  nome CHAR(30),
  saldo INT(4),
  limite INT(4),
  PRIMARY KEY (id)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

CREATE TABLE transacoes (
  id INT(4) AUTO_INCREMENT,
  cliente_id INT(1),
  valor SMALLINT(2),
  tipo CHAR(1),
  descricao CHAR(10),
  realizada_em TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (id),
  KEY cliente_id_fk_idx (cliente_id),
  CONSTRAINT cliente_id_fk FOREIGN KEY (cliente_id) REFERENCES clientes(id)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

INSERT INTO clientes (nome, saldo, limite) VALUES
  ('pedrin', 0, 100000),
  ('jorge', 0, 80000),
  ('ana', 0, 1000000),
  ('tico', 0, 10000000),
  ('jao', 0, 500000);

