package widgets

import (
	"strings"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

// ScriptMessage represents a lightweight JS bridge payload for the verified Go baseline.
type ScriptMessage struct {
	Channel string
	Payload string
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

	OnNavigate      func(url string)
	OnLoad          func(url string)
	OnTitleChanged  func(title string)
	OnHistoryChanged func(index int, length int)
	OnScriptMessage func(msg ScriptMessage)

	LastEvaluatedJS string
	LastPostedMsg   ScriptMessage
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

// EvalJS establishes the contract for a future real JS bridge.
// In the verified baseline this records the latest expression and fires a synthetic callback.
func (w *WebView) EvalJS(source string) {
	w.LastEvaluatedJS = source
	if w.OnScriptMessage != nil {
		w.LastPostedMsg = ScriptMessage{Channel: "eval", Payload: source}
		w.OnScriptMessage(w.LastPostedMsg)
	}
}

// PostMessage simulates a page -> host bridge message in the verified baseline.
func (w *WebView) PostMessage(channel, payload string) {
	w.LastPostedMsg = ScriptMessage{Channel: channel, Payload: payload}
	if w.OnScriptMessage != nil {
		w.OnScriptMessage(w.LastPostedMsg)
	}
}

func (w *WebView) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
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
		layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return list.Layout(gtx, len(lines), func(gtx layout.Context, i int) layout.Dimensions {
				return material.Label(mth, unit.Sp(11), lines[i]).Layout(gtx)
			})
		}),
	)
}
