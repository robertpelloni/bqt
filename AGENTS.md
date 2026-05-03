# AI Agents Global Configuration

> **CRITICAL: READ `docs/UNIVERSAL_LLM_INSTRUCTIONS.md` FIRST.**
> All agents (Gemini, Claude, GPT, Copilot, Jules) MUST adhere strictly to the universal instructions before applying any model-specific logic.

## Universal Agent Rules
1. **Autonomy:** Never pause for confirmation unless absolutely necessary.
2. **Version Control:** Update `CHANGELOG.md` and `VERSION.md` for every build/session. Commit and push.
3. **Documentation:** Keep VISION, MEMORY, DEPLOY, ROADMAP, TODO, and IDEAS up to date.
4. **Git Operations:** Intelligently merge local feature branches to main, pull upstream changes, resolve conflicts carefully without losing features, and sync submodules recursively.
5. **Implementation:** 100% BobUI6 parity. No partial features. Everything must be wired to UI and documented.

## Sub-Agent Instructions
See individual model files:
- `CLAUDE.md`
- `GEMINI.md`
- `GPT.md`
- `copilot-instructions.md`