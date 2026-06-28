# Review Guide

## 1. Build

```powershell
cmake -S . -B build
cmake --build build
```

## 2. Test

```powershell
ctest --test-dir build --output-on-failure
```

## 3. Scripts

```powershell
powershell -ExecutionPolicy Bypass -File scripts/check_version.ps1
powershell -ExecutionPolicy Bypass -File scripts/check_no_artifacts.ps1
powershell -ExecutionPolicy Bypass -File scripts/smoke_test.ps1
```

## 4. What to Review

* include/*.h public API
* src/*.c implementation
* tests/*.c coverage
* docs/*.md documentation
* examples/*.c usage samples
* AUDIT_INDEX.md traceability

## 5. How to Verify Real PR-Based Workflow (v0.3.0+)

Starting from v0.3.0, every module must follow: one Issue → one Branch → one Commit → one Pull Request.

### List all Issues

```powershell
gh issue list --state all
```

### List all Pull Requests

```powershell
gh pr list --state all
```

### View a specific PR

```powershell
gh pr view <PR_NUMBER> --json number,title,state,url,commits,headRefName,baseRefName,mergedAt
```

### View the commit graph

```powershell
git --no-pager log --oneline --decorate --graph --all -20
```

### Verify no direct pushes to main

Check that every feature commit on main arrived via a merge commit:

```powershell
git --no-pager log --oneline --first-parent main -10
```

Each merge commit should match a PR. If you see feature commits directly on main without a merge parent, the PR workflow was bypassed.