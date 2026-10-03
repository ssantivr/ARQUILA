# Context & Objective
Act as an expert software engineer and technical lead. Your goal is to review the existing project structure (backend, frontend, data_structures, and docs) with minimum token consumption, and implement the core components required to reach 55% project completion.

## Token Efficiency Rules (CRITICAL)
1. DO NOT read entire large files or dump whole directories into context. 
2. Use targeted file inspections (read only headers, imports, or specific line ranges using tool calls).
3. Work incrementally: focus only on the immediate files needed for the current task.

## Execution Scope (Target: 55% Completion)
1. **Data Structures Implementation**: Complete missing logic in `data_structures/` (SinglyLinkedList, DoublyLinkedList, Stack, Queue, ArrayExamples) ensuring clean C++ code.
2. **Backend Setup**: Verify `backend/app/main.py`, requirements, and ensure the FastAPI health check test (`backend/tests/test_health.py`) passes successfully.
3. **Database Schema**: Review `database/schema.sql` and `database/seed.sql` to align with the core backend models.

## Instructions
- Start by listing the workspace structure silently or checking specific files using glob patterns (`data_structures/**/*.cpp`, `backend/app/*.py`).
- Implement code solutions in **English** (backend/C++ source code).
- Provide brief status updates or explanations in **Spanish** only when requested or at final summary.
- Begin execution immediately with step 1.
