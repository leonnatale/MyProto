# MyProto

## Descrição do Projeto

Este projeto é um estudo acadêmico sobre protocolos de rede, implementado em linguagem C. O objetivo é demonstrar os conceitos fundamentais de comunicação em rede através de uma implementação simplificada de um protocolo personalizado chamado "MyProto". O código explora aspectos como empacotamento de dados, transmissão e recepção de mensagens, servindo como uma base educacional para entender como os protocolos de rede operam em um nível baixo.

**Nota importante:** Este projeto é exclusivamente para fins de estudo e aprendizado. Não haverá atualizações futuras, correções de bugs ou melhorias. Ele representa um estudo e não deve ser usado em ambientes de produção ou projetos reais.

## Como Funciona

O projeto implementa um protocolo de rede simples onde:

1. O programa principal (`main.c`) inicializa o contexto do protocolo.
2. As funções em `myproto.c` lidam com a criação, envio e recebimento de pacotes de dados.
3. O protocolo define estruturas para mensagens, incluindo cabeçalhos e payloads.
4. A comunicação é simulada localmente, demonstrando o fluxo de dados sem conexão real à rede.

### Diagrama Simples do Funcionamento

```
[Programa Principal - main.c]
          |
          | Inicializa protocolo
          v
[MyProto - myproto.c/h]
          |
          | Cria pacote de dados
          v
[Envio/Recepção Simulada]
          |
          | Processa resposta
          v
[Saída/Resultado]
```

- **Inicialização**: O `main.c` configura o ambiente e chama funções de inicialização do MyProto.
- **Empacotamento**: O MyProto constrói pacotes com cabeçalhos (tipo, tamanho) e dados úteis.
- **Transmissão**: Simulação do envio e recebimento de pacotes (em um estudo real, isso envolveria sockets de rede).
- **Processamento**: Análise e resposta aos pacotes recebidos.

Este diagrama ilustra o fluxo unidirecional básico; em protocolos reais, haveria handshakes bidirecionais e tratamento de erros.

## Compilação e Execução

Para compilar o projeto usando CMake:

```bash
mkdir build
cd build
cmake ..
make
./MyProto
```

## Considerações Finais

Este é um projeto puramente educacional para ilustrar conceitos de protocolos de rede. Não inclui recursos avançados como criptografia, autenticação ou tratamento robusto de erros. Como mencionado, não serão feitas atualizações, e o código serve apenas como material de estudo.
