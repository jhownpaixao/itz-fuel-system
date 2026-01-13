# Changelog - iTz Fuel System

Todas as mudanças notáveis neste projeto serão documentadas neste arquivo.

O formato é baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.0.0/),
e este projeto adere ao [Semantic Versioning](https://semver.org/lang/pt-BR/).

---

## [1.0.1] - 2026-01-13

### Corrigido
- **NULL Pointer no LogFuelStation**: Corrigido erro de NULL pointer na função `LogFuelStation` que causava crash durante inicialização do servidor
  - O erro ocorria ao tentar acessar `station.m_Variables` sem verificar se era NULL
  - Gerava múltiplas entradas idênticas no crash.log (uma por estação carregada)
  - Stack trace: `fs_module.c:876 -> LoadStationProfiles:371 -> OnMissionStart:61`
  - Arquivo afetado: [FS_Module.c](Scripts/4_World/Modules/FS_Module.c:876)

---

## [1.0.0] - 2026-01-13

### Adicionado
- Lançamento inicial do iTz Fuel System
- Sistema completo de gerenciamento de combustível para DayZ
- **FS_PowerGenerator**: Gerador de energia customizado
  - Capacidade do tanque: 5000ml
  - Modelo 3D com texturas customizadas
  - Sistema de energia integrado
- **Sistema de Postos de Combustível**
  - Suporte a múltiplas estações no mapa
  - Sistema de perfis de combustível
  - Configuração de alcance e requisitos de energia
  - Sistema de respawn de combustível
- **Ações de Abastecimento**
  - Abastecer veículos (carros e barcos)
  - Abastecer geradores
  - Encher recipientes com combustível
  - Medir combustível disponível
- **Sistema de Sons**
  - Sons de bomba de combustível (2 variações)
  - Sons de alavanca da bomba
  - Sons ambiente realistas
- **Suporte a Veículos**
  - Integração com CarScript
  - Integração com BoatScript
  - Suporte para mod RF (RFWC_base)
- **Combustíveis Customizados**
  - KeroseneLiquid (Combustível de Aviação)
  - Sistema de toxicidade e flamabilidade
- **Recipientes Compatíveis**
  - Barris, galões de gasolina
  - Panelas, caldeirões, cantis
  - Garrafas de água e vidro
  - Bolsas de água
- **Sistema de Perfis**
  - Perfis de estações de combustível (JSON)
  - Perfis de veículos (JSON)
  - Gerador automático de perfis de exemplo
  - Sistema de migração de versões antigas
- **Ferramentas de Desenvolvimento**
  - Sistema de logging com níveis (0=Error, 1=Info, 2=Trace)
  - Manipulação de arquivos XML
  - Sistema de sincronização cliente-servidor
- **Localização**
  - Stringtable para suporte multilíngue

### Técnico
- Dependências: DZ_Data, DZ_Scripts, JM_CF_Scripts
- Módulos: Core, Game, World, Mission
- Sistema de componentes de ação contínua
- Sistema de dano configurável
- EnergyManager para gestão de energia do gerador

### Créditos
- **Autor**: iTzChronuZ
- **Nome**: Jhonnata Paixão
- **SteamID**: 76561199441861559
- **Versão**: 1.0.0
