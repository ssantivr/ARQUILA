# Context & Objective
Act as an expert full-stack engineer and technical lead. Your goal is to review the project, enforce a strict technology stack—**Python for the backend** and **TypeScript for the frontend**—and correct or refactor any code, files, or configurations that do not comply with this stack. Progress the project efficiently to reach 55% completion.

## Token Efficiency Rules (CRITICAL)
1. DO NOT read entire large directories or dump whole files into context. 
2. Use targeted file inspections (read only headers, package files, or specific line ranges using tool calls).
3. Work incrementally: focus only on the immediate files needed for the current task.

## Stack Enforcement & Correction Scope (Target: 55% Completion)
1. **Tech Stack Audit & Correction**: Inspect the workspace. If you find backend logic, scripts, or components written in incorrect languages or frameworks, **refactor and correct them** so the backend remains exclusively in **Python (FastAPI)** and the frontend in **TypeScript**.
2. **Python Backend**: Verify `backend/app/main.py`, requirements, and ensure the health check test (`backend/tests/test_health.py`) passes successfully.
3. **TypeScript Frontend**: Check `frontend/package.json`, `frontend/tsconfig.json`, and core components in `frontend/src/`, ensuring strict typing and proper API communication with the Python backend.

## Instructions
- Start by selectively listing workspace files using glob patterns (`backend/app/*.py`, `frontend/src/**/*.ts`).
- Automatically detect and fix any technology mismatches or misplaced logic found outside Python/TypeScript.
- Write backend code in **Python** and frontend code in **TypeScript** (English for code, variables, and technical comments).
- Provide brief status updates or explanations in **Spanish** only when requested or at final summary.
- Begin execution immediately with step 1.
