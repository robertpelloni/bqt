package widgets

import (
	"encoding/json"
	"fmt"
	"strings"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

// ScriptMessage represents a lightweight page-to-host or host-to-page bridge payload.
type ScriptMessage struct {
	ID      string `json:"id"`
	Channel string `json:"channel"`
	Payload string `json:"payload"`
	Kind    string `json:"kind"` // emit | eval | request | reply | error
}

type JSBridgeHandler func(payload string) (string, error)

// JSBridge provides a compile-safe contract for WebEngineQuick-style host/page messaging.
type JSBridge struct {
	OnMessage func(msg ScriptMessage)
	Handlers  map[string]JSBridgeHandler
	nextID    int
}

func (b *JSBridge) ensureHandlers() {
	if b.Handlers == nil {
		b.Handlers = make(map[string]JSBridgeHandler)
	}
}

func (b *JSBridge) nextMessageID() string {
	b.nextID++
	return fmt.Sprintf("msg-%d", b.nextID)
}

func (b *JSBridge) RegisterHandler(channel string, handler JSBridgeHandler) {
	b.ensureHandlers()
	b.Handlers[channel] = handler
}

func (b *JSBridge) Emit(channel, payload string) {
	if b == nil || b.OnMessage == nil {
		return
	}
	b.OnMessage(ScriptMessage{ID: b.nextMessageID(), Channel: channel, Payload: payload, Kind: "emit"})
}

func (b *JSBridge) Eval(source string) {
	if b == nil || b.OnMessage == nil {
		return
	}
	b.OnMessage(ScriptMessage{ID: b.nextMessageID(), Channel: "eval", Payload: source, Kind: "eval"})
}

func (b *JSBridge) Request(channel, payload string) {
	if b == nil || b.OnMessage == nil {
		return
	}
	b.OnMessage(ScriptMessage{ID: b.nextMessageID(), Channel: channel, Payload: payload, Kind: "request"})
}

// Dispatch routes a page->host request into a registered handler and returns a reply/error.
func (b *JSBridge) Dispatch(channel, payload string) ScriptMessage {
	b.ensureHandlers()
	id := b.nextMessageID()
	h, ok := b.Handlers[channel]
	if !ok {
		return ScriptMessage{ID: id, Channel: channel, Payload: "handler not found", Kind: "error"}
	}
	result, err := h(payload)
	if err != nil {
		return ScriptMessage{ID: id, Channel: channel, Payload: err.Error(), Kind: "error"}
	}
	return ScriptMessage{ID: id, Channel: channel, Payload: result, Kind: "reply"}
}

// WebView is a lightweight WebEngineQuick-style navigation model for the verified Go baseline.
// It is intentionally not a full browser engine; it provides URL/history/title/content semantics
// plus a compile-safe event/bridge contract the rest of the framework can build on.
type WebView struct {
	URL     string
	Title   string
	HTML    string
	History []string
	Index   int

	BackBtn    widget.Clickable
	ForwardBtn widget.Clickable
	ReloadBtn  widget.Clickable

	OnNavigate       func(url string)
	OnLoad           func(url string)
	OnTitleChanged   func(title string)
	OnHistoryChanged func(index int, length int)
	OnScriptMessage  func(msg ScriptMessage)

	Bridge *JSBridge

	LastEvaluatedJS string
	LastPostedMsg   ScriptMessage
}

func (w *WebView) ensureBridge() {
	if w.Bridge == nil {
		w.Bridge = &JSBridge{}
	}
	w.Bridge.ensureHandlers()
	w.Bridge.OnMessage = func(msg ScriptMessage) {
		w.LastPostedMsg = msg
		if w.OnScriptMessage != nil {
			w.OnScriptMessage(msg)
		}
	}
}

func (w *WebView) RegisterHandler(channel string, handler JSBridgeHandler) {
	w.ensureBridge()
	w.Bridge.RegisterHandler(channel, handler)
}

func (w *WebView) Navigate(url, html string) {
	w.URL = url
	w.Title = url
	w.HTML = html
	if w.Index < len(w.History)-1 {
		w.History = w.History[:w.Index+1]
	}
	w.History = append(w.History, url)
	w.Index = len(w.History) - 1
	if w.OnNavigate != nil {
		w.OnNavigate(url)
	}
	if w.OnLoad != nil {
		w.OnLoad(url)
	}
	if w.OnTitleChanged != nil {
		w.OnTitleChanged(w.Title)
	}
	if w.OnHistoryChanged != nil {
		w.OnHistoryChanged(w.Index, len(w.History))
	}
}

func (w *WebView) Back() {
	if w.Index > 0 {
		w.Index--
		w.URL = w.History[w.Index]
		w.Title = w.URL
		if w.OnNavigate != nil {
			w.OnNavigate(w.URL)
		}
		if w.OnTitleChanged != nil {
			w.OnTitleChanged(w.Title)
		}
		if w.OnHistoryChanged != nil {
			w.OnHistoryChanged(w.Index, len(w.History))
		}
	}
}

func (w *WebView) Forward() {
	if w.Index >= 0 && w.Index < len(w.History)-1 {
		w.Index++
		w.URL = w.History[w.Index]
		w.Title = w.URL
		if w.OnNavigate != nil {
			w.OnNavigate(w.URL)
		}
		if w.OnTitleChanged != nil {
			w.OnTitleChanged(w.Title)
		}
		if w.OnHistoryChanged != nil {
			w.OnHistoryChanged(w.Index, len(w.History))
		}
	}
}

// EvalJS is a host-to-page bridge request.
func (w *WebView) EvalJS(source string) {
	w.ensureBridge()
	w.LastEvaluatedJS = source
	w.Bridge.Eval(source)
}

// PostMessage is a page-to-host bridge message.
func (w *WebView) PostMessage(channel, payload string) {
	w.ensureBridge()
	w.Bridge.Emit(channel, payload)
}

// Request sends a request and immediately routes it through the local bridge handler set.
func (w *WebView) Request(channel, payload string) ScriptMessage {
	w.ensureBridge()
	reply := w.Bridge.Dispatch(channel, payload)
	w.LastPostedMsg = reply
	if w.OnScriptMessage != nil {
		w.OnScriptMessage(reply)
	}
	return reply
}

// BridgeContractJSON exposes the bridge contract in a tool-friendly format.
func (w *WebView) BridgeContractJSON() string {
	contract := map[string]any{
		"supports": []string{"navigate", "history", "titleChanged", "load", "scriptMessage", "evalJS", "postMessage", "request", "reply", "error"},
		"message": map[string]string{
			"id":      "optional request/reply correlation id",
			"channel": "logical route name",
			"payload": "string payload",
			"kind":    "emit|eval|request|reply|error",
		},
	}
	data, _ := json.Marshal(contract)
	return string(data)
}

func (w *WebView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	w.ensureBridge()

	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	if w.BackBtn.Clicked(gtx) {
		w.Back()
	}
	if w.ForwardBtn.Clicked(gtx) {
		w.Forward()
	}
	if w.ReloadBtn.Clicked(gtx) {
		if w.OnLoad != nil {
			w.OnLoad(w.URL)
		}
	}

	lines := strings.Split(w.HTML, "<br>")
	list := layout.List{Axis: layout.Vertical}

	return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Horizontal}.Layout(gtx,
				layout.Rigid(material.Button(mth, &w.BackBtn, "<").Layout),
				layout.Rigid(material.Button(mth, &w.ForwardBtn, ">").Layout),
				layout.Rigid(material.Button(mth, &w.ReloadBtn, "Reload").Layout),
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					return material.Label(mth, unit.Sp(10), w.URL).Layout(gtx)
				}),
			)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.H6(mth, w.Title).Layout(gtx)
		}),
		layout.Rigid(func(gtx layout.Context) layout.Dimensions {
			return material.Caption(mth, w.BridgeContractJSON()).Layout(gtx)
		}),
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
				return material.Label(mth, unit.Sp(11), lines[i]).Layout(gtx)
			})
		}),
	)
}
