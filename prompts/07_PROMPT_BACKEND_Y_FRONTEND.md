# Context & Objective
Act as an expert full-stack engineer and technical lead. Your goal is to review the existing project structure (Python backend and TypeScript frontend) with minimum token consumption, and implement the core components required to reach 55% project completion.

## Token Efficiency Rules (CRITICAL)
1. DO NOT read entire large directories or dump whole files into context. 
2. Use targeted file inspections (read only headers, package files, or specific line ranges using tool calls).
3. Work incrementally: focus only on the immediate files needed for the current task.

## Execution Scope (Target: 55% Completion)
1. **Python Backend**: Verify `backend/app/main.py`, requirements, FastAPI setup, and ensure the health check test (`backend/tests/test_health.py`) passes successfully.
2. **TypeScript Frontend**: Check `frontend/package.json`, `frontend/tsconfig.json`, and core components in `frontend/src/` to align API calls with the Python backend.
3. **Database & Data Structures**: Review schema scripts (`database/schema.sql`) and C++ data structures (`data_structures/`) if required for backend integration.

## Instructions
- Start by listing workspace files selectively using glob patterns (`backend/app/*.py`, `frontend/src/**/*.ts`).
- Write backend code in **Python** and frontend code in **TypeScript** (English for code/variables/comments).
- Provide brief status updates or explanations in **Spanish** only when requested or at final summary.
- Begin execution immediately with step 1.
