# Padrão de commits

Este projeto segue o padrão **Conventional Commits** com emojis, baseado no guia [iuricode/padroes-de-commits](https://github.com/iuricode/padroes-de-commits).

## Formato

```
:emoji: tipo: Descrição curta
```

- Primeira linha curta e objetiva.
- Para explicar mais, use o corpo do commit (segunda linha em diante).
- Links sem encurtadores.

## Tipos e emojis

| Tipo | Emoji | Quando usar |
| --- | --- | --- |
| `feat` | ✨ `:sparkles:` | Novo recurso |
| `fix` | 🐛 `:bug:` | Correção de bug |
| `docs` | 📚 `:books:` | Documentação (README, comentários) |
| `style` | 👌 `:ok_hand:` | Formatação, sem mudar lógica |
| `refactor` | ♻️ `:recycle:` | Refatoração sem mudar funcionalidade |
| `perf` | ⚡ `:zap:` | Melhoria de performance |
| `test` | 🧪 `:test_tube:` | Testes |
| `build` | 📦 `:package:` | Dependências e arquivos de build |
| `chore` | 🔧 `:wrench:` | Configuração e tarefas gerais |
| `ci` | 🧱 `:bricks:` | Integração contínua e infraestrutura |
| `cleanup` | 🧹 `:broom:` | Limpeza de código |
| `remove` | 🗑️ `:wastebasket:` | Remoção de arquivos ou recursos |
| `init` | 🎉 `:tada:` | Commit inicial |
| segurança | 🔒️ `:lock:` | Credenciais, senhas, proteção |

## Sequência sugerida para este projeto

```bash
git init
git branch -M main

git add LICENSE .gitignore
git commit -m ":tada: Commit inicial"

git add oraculo_generativo/oraculo_generativo.ino
git commit -m ":sparkles: feat: Gramática generativa e Jogo da Vida no OLED"

git add oraculo_generativo/secrets.example.h
git commit -m ":lock: chore: Credenciais Wi-Fi movidas para secrets.h"

git add README.md COMMITS.md
git commit -m ":books: docs: README e padrão de commits"

git remote add origin https://github.com/maxwell-franca/esp32-oraculo-generativo.git
git push -u origin main
```

## Exemplos para o dia a dia

| Comando | Resultado |
| --- | --- |
| `git commit -m ":bug: fix: Frase repetindo o mesmo substantivo"` | 🐛 fix: Frase repetindo o mesmo substantivo |
| `git commit -m ":sparkles: feat: Botão para frase em modo noturno"` | ✨ feat: Botão para frase em modo noturno |
| `git commit -m ":zap: perf: Menos redesenho do display"` | ⚡ perf: Menos redesenho do display |
| `git commit -m ":recycle: refactor: Separando a gramática em arquivo próprio"` | ♻️ refactor: Separando a gramática em arquivo próprio |
| `git commit -m ":books: docs: Foto da montagem no README"` | 📚 docs: Foto da montagem no README |
| `git commit -m ":broom: cleanup: Removendo comentários antigos"` | 🧹 cleanup: Removendo comentários antigos |

## Antes de cada commit

1. Rode `git status` e confira se `secrets.h` **não** aparece na lista.
2. Rode `git diff --staged` para revisar o que vai ser enviado.
3. Escreva a mensagem no padrão acima.
