# WebView Navigation and History Tests

## Date
2026-04-05

## Scope
This note documents additional automated tests for the Go `WebView` surface, focused on navigation and history behavior.

Affected file:
- `internal/ui/widgets/webview_test.go`

## Goal
Validate that the non-bridge parts of the `WebView` surface also behave as documented, especially:
- callback firing
- history index tracking
- back/forward transitions
- forward-history truncation when navigating from the middle of the stack

## Test coverage added
### 1. Navigate callback/state behavior
Verified that `Navigate(...)`:
- updates URL/title/HTML
- appends to history
- updates history index
- fires `OnNavigate`
- fires `OnLoad`
- fires `OnTitleChanged`
- fires `OnHistoryChanged`

### 2. Back/forward and truncation behavior
Verified that:
- `Back()` moves to the previous history entry
- `Forward()` moves to the next history entry
- navigating after moving back truncates forward history
- load callbacks continue to fire only on actual `Navigate(...)` calls in the current baseline
- history callback state matches the final index/length

## Commands
```bash
go test ./internal/...
go build -buildvcs=false .
```

## Result
Both commands succeeded.

## Findings
The `WebView` surface now has automated coverage for both:
- bridge/runtime behavior
- navigation/history behavior

This makes the Go `WebView` one of the better-validated primitives in the current BobUI baseline.
