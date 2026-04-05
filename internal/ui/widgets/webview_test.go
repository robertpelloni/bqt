package widgets

import (
	"errors"
	"fmt"
	"testing"
)

func TestWebViewRequestAutoDispatchCorrelatesReply(t *testing.T) {
	w := &WebView{}
	var seen []ScriptMessage
	w.OnScriptMessage = func(msg ScriptMessage) {
		seen = append(seen, msg)
	}
	w.RegisterHandler("ping", func(payload string) (string, error) {
		return "pong:" + payload, nil
	})

	reply := w.Request("ping", "hello")
	if reply.Kind != "reply" {
		t.Fatalf("expected reply kind, got %q", reply.Kind)
	}
	if reply.Payload != "pong:hello" {
		t.Fatalf("expected reply payload pong:hello, got %q", reply.Payload)
	}
	if reply.Channel != "ping" {
		t.Fatalf("expected reply channel ping, got %q", reply.Channel)
	}
	if reply.ID == "" {
		t.Fatal("expected correlated reply id to be set")
	}
	if w.QueueDepth() != 0 {
		t.Fatalf("expected empty queue after auto-dispatch, got %d", w.QueueDepth())
	}
	if w.LastReply.ID != reply.ID {
		t.Fatalf("expected LastReply id %q, got %q", reply.ID, w.LastReply.ID)
	}
	if len(seen) < 2 {
		t.Fatalf("expected at least request and reply script messages, got %d", len(seen))
	}
	if seen[0].Kind != "request" {
		t.Fatalf("expected first seen message kind request, got %q", seen[0].Kind)
	}
	if seen[len(seen)-1].Kind != "reply" {
		t.Fatalf("expected last seen message kind reply, got %q", seen[len(seen)-1].Kind)
	}
}

func TestWebViewManualPumpProcessesQueuedRequest(t *testing.T) {
	w := &WebView{}
	w.SetAutoDispatch(false)
	w.RegisterHandler("ping", func(payload string) (string, error) {
		return "pong:" + payload, nil
	})

	pending := w.Request("ping", "queued")
	if pending.Kind != "request" {
		t.Fatalf("expected pending request kind, got %q", pending.Kind)
	}
	if pending.Payload != "pending runtime reply" {
		t.Fatalf("expected pending runtime reply payload, got %q", pending.Payload)
	}
	if w.QueueDepth() != 1 {
		t.Fatalf("expected queue depth 1 before pump, got %d", w.QueueDepth())
	}

	replies := w.PumpRuntime()
	if len(replies) != 1 {
		t.Fatalf("expected 1 reply from pump, got %d", len(replies))
	}
	if replies[0].ID != pending.ID {
		t.Fatalf("expected reply id %q, got %q", pending.ID, replies[0].ID)
	}
	if replies[0].Payload != "pong:queued" {
		t.Fatalf("expected pumped reply payload pong:queued, got %q", replies[0].Payload)
	}
	if w.QueueDepth() != 0 {
		t.Fatalf("expected empty queue after pump, got %d", w.QueueDepth())
	}
}

func TestWebViewEvalJSUsesEvalHandler(t *testing.T) {
	w := &WebView{}
	w.RegisterEvalHandler(func(payload string) (string, error) {
		return "evaluated:" + payload, nil
	})

	w.EvalJS("window.answer = 42")
	if w.LastEvaluatedJS != "window.answer = 42" {
		t.Fatalf("expected LastEvaluatedJS to be tracked, got %q", w.LastEvaluatedJS)
	}
	if w.LastEvalResult != "evaluated:window.answer = 42" {
		t.Fatalf("expected LastEvalResult to be tracked, got %q", w.LastEvalResult)
	}
	if w.LastReply.Kind != "reply" {
		t.Fatalf("expected eval reply kind, got %q", w.LastReply.Kind)
	}
	if w.LastReply.Channel != "eval" {
		t.Fatalf("expected eval reply channel, got %q", w.LastReply.Channel)
	}
}

func TestWebViewErrorPathsAreRecorded(t *testing.T) {
	w := &WebView{}

	missing := w.Request("missing", "payload")
	if missing.Kind != "error" {
		t.Fatalf("expected missing handler error, got %q", missing.Kind)
	}
	if missing.Payload != "handler not found" {
		t.Fatalf("expected handler not found payload, got %q", missing.Payload)
	}

	w.RegisterHandler("boom", func(payload string) (string, error) {
		return "", errors.New("boom failure")
	})
	boom := w.Request("boom", "payload")
	if boom.Kind != "error" {
		t.Fatalf("expected handler error kind, got %q", boom.Kind)
	}
	if boom.Payload != "boom failure" {
		t.Fatalf("expected boom failure payload, got %q", boom.Payload)
	}

	w.EvalJS("window.fail = true")
	if w.LastReply.Kind != "error" {
		t.Fatalf("expected eval without handler to produce error, got %q", w.LastReply.Kind)
	}
	if w.LastReply.Payload != "no eval handler registered" {
		t.Fatalf("expected missing eval handler payload, got %q", w.LastReply.Payload)
	}
}

func TestHandleScriptMessageSupportsExternalRepliesAndRequests(t *testing.T) {
	w := &WebView{}
	w.RegisterHandler("title", func(payload string) (string, error) {
		w.Title = payload
		return "title-updated", nil
	})

	reply := w.HandleScriptMessage(ScriptMessage{Channel: "title", Payload: "external-title", Kind: "request"})
	if reply.Kind != "reply" {
		t.Fatalf("expected external request reply kind, got %q", reply.Kind)
	}
	if reply.Payload != "title-updated" {
		t.Fatalf("expected external request reply payload, got %q", reply.Payload)
	}
	if w.Title != "external-title" {
		t.Fatalf("expected handler to update title, got %q", w.Title)
	}

	externalReply := w.HandleScriptMessage(ScriptMessage{Channel: "eval", Payload: "ok", Kind: "reply"})
	if externalReply.Kind != "reply" {
		t.Fatalf("expected external reply kind, got %q", externalReply.Kind)
	}
	if externalReply.ID == "" {
		t.Fatal("expected external reply without id to be assigned an id")
	}
	if w.LastReply.ID != externalReply.ID {
		t.Fatalf("expected LastReply id %q, got %q", externalReply.ID, w.LastReply.ID)
	}
}

func TestWebViewNavigateFiresCallbacksAndTracksHistory(t *testing.T) {
	w := &WebView{}
	var navigated []string
	var loaded []string
	var titles []string
	var histories []string

	w.OnNavigate = func(url string) {
		navigated = append(navigated, url)
	}
	w.OnLoad = func(url string) {
		loaded = append(loaded, url)
	}
	w.OnTitleChanged = func(title string) {
		titles = append(titles, title)
	}
	w.OnHistoryChanged = func(index int, length int) {
		histories = append(histories, fmt.Sprintf("%d/%d", index, length))
	}

	w.Navigate("page:a", "A")
	w.Navigate("page:b", "B")

	if w.URL != "page:b" {
		t.Fatalf("expected current URL page:b, got %q", w.URL)
	}
	if w.Title != "page:b" {
		t.Fatalf("expected current Title page:b, got %q", w.Title)
	}
	if w.HTML != "B" {
		t.Fatalf("expected HTML B, got %q", w.HTML)
	}
	if w.Index != 1 {
		t.Fatalf("expected history index 1, got %d", w.Index)
	}
	if len(w.History) != 2 || w.History[0] != "page:a" || w.History[1] != "page:b" {
		t.Fatalf("unexpected history contents: %+v", w.History)
	}
	if len(navigated) != 2 || navigated[0] != "page:a" || navigated[1] != "page:b" {
		t.Fatalf("unexpected navigate callbacks: %+v", navigated)
	}
	if len(loaded) != 2 || loaded[0] != "page:a" || loaded[1] != "page:b" {
		t.Fatalf("unexpected load callbacks: %+v", loaded)
	}
	if len(titles) != 2 || titles[0] != "page:a" || titles[1] != "page:b" {
		t.Fatalf("unexpected title callbacks: %+v", titles)
	}
	if len(histories) != 2 || histories[0] != "0/1" || histories[1] != "1/2" {
		t.Fatalf("unexpected history callbacks: %+v", histories)
	}
}

func TestWebViewBackForwardAndTruncateHistory(t *testing.T) {
	w := &WebView{}
	var navigated []string
	var loaded []string
	var histories []string

	w.OnNavigate = func(url string) {
		navigated = append(navigated, url)
	}
	w.OnLoad = func(url string) {
		loaded = append(loaded, url)
	}
	w.OnHistoryChanged = func(index int, length int) {
		histories = append(histories, fmt.Sprintf("%d/%d", index, length))
	}

	w.Navigate("page:a", "A")
	w.Navigate("page:b", "B")
	w.Navigate("page:c", "C")
	w.Back()
	if w.URL != "page:b" || w.Index != 1 {
		t.Fatalf("expected Back to reach page:b at index 1, got url=%q index=%d", w.URL, w.Index)
	}
	w.Forward()
	if w.URL != "page:c" || w.Index != 2 {
		t.Fatalf("expected Forward to reach page:c at index 2, got url=%q index=%d", w.URL, w.Index)
	}
	w.Back()
	w.Navigate("page:d", "D")

	if w.URL != "page:d" || w.Index != 2 {
		t.Fatalf("expected navigate from middle to end at page:d index 2, got url=%q index=%d", w.URL, w.Index)
	}
	if len(w.History) != 3 {
		t.Fatalf("expected truncated history length 3, got %d", len(w.History))
	}
	if w.History[0] != "page:a" || w.History[1] != "page:b" || w.History[2] != "page:d" {
		t.Fatalf("unexpected truncated history contents: %+v", w.History)
	}
	if len(loaded) != 4 {
		t.Fatalf("expected load callback only on Navigate calls (4 total), got %d", len(loaded))
	}
	if loaded[3] != "page:d" {
		t.Fatalf("expected last load callback page:d, got %q", loaded[3])
	}
	if len(navigated) < 6 {
		t.Fatalf("expected navigate callbacks across navigate/back/forward flow, got %d", len(navigated))
	}
	if histories[len(histories)-1] != "2/3" {
		t.Fatalf("expected final history callback 2/3, got %+v", histories)
	}
}
