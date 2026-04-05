package ui

import (
	"fmt"
	"log"

	"gioui.org/app"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/unit"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	shell *widgets.Shell
	login *widgets.LoginView
	demo  *widgets.DemoSurface
	th    theme.Theme
}

func NewEngine() *Engine {
	return &Engine{
		shell: &widgets.Shell{},
		login: &widgets.LoginView{},
		demo:  widgets.NewDemoSurface(),
		th:    theme.GetTheme(theme.Cyberpunk),
	}
}

// Run now opens a real Gio window/frame loop so the current BobUI Go baseline
// can be exercised as an actual runtime surface instead of only a safe init path.
func (e *Engine) Run() error {
	log.Println("OmniUI Go: Engine starting live Gio runtime.")
	e.initializeDemo()

	w := new(app.Window)
	w.Option(
		app.Title("BobUI Go Runtime Demo"),
		app.Size(unit.Dp(1280), unit.Dp(900)),
	)

	var ops op.Ops
	for {
		switch ev := w.Event().(type) {
		case app.DestroyEvent:
			if ev.Err != nil {
				log.Printf("BOBUI RUNTIME: window destroyed with error: %v", ev.Err)
			}
			return ev.Err
		case app.FrameEvent:
			gtx := app.NewContext(&ops, ev)
			e.layout(gtx)
			ev.Frame(gtx.Ops)
		}
	}
}

func (e *Engine) initializeDemo() {
	if e.demo == nil {
		return
	}
	e.demo.WebView.OnNavigate = func(url string) {
		log.Printf("BOBUI WEBVIEW: navigate -> %s", url)
	}
	e.demo.WebView.OnLoad = func(url string) {
		log.Printf("BOBUI WEBVIEW: load -> %s", url)
	}
	e.demo.WebView.OnTitleChanged = func(title string) {
		log.Printf("BOBUI WEBVIEW: title -> %s", title)
	}
	e.demo.WebView.OnHistoryChanged = func(index int, length int) {
		log.Printf("BOBUI WEBVIEW: history index=%d length=%d", index, length)
	}
	e.demo.WebView.OnScriptMessage = func(msg widgets.ScriptMessage) {
		log.Printf("BOBUI WEBVIEW: script message kind=%s id=%s channel=%s payload=%s", msg.Kind, msg.ID, msg.Channel, msg.Payload)
	}
	e.demo.WebView.RegisterHandler("ping", func(payload string) (string, error) {
		return fmt.Sprintf("pong:%s", payload), nil
	})
	e.demo.WebView.RegisterHandler("title", func(payload string) (string, error) {
		e.demo.WebView.Title = payload
		if e.demo.WebView.OnTitleChanged != nil {
			e.demo.WebView.OnTitleChanged(payload)
		}
		return "title-updated", nil
	})
	e.demo.WebView.RegisterEvalHandler(func(payload string) (string, error) {
		return fmt.Sprintf("evaluated:%s", payload), nil
	})
	e.demo.WebView.PostMessage("boot", "demo-surface-online")
	e.demo.WebView.EvalJS("window.bobuiReady = true")
	_ = e.demo.WebView.Request("ping", "hello")
	_ = e.demo.WebView.HandleScriptMessage(widgets.ScriptMessage{Channel: "title", Payload: "about:bobui runtime", Kind: "request"})
}

func (e *Engine) layout(gtx layout.Context) layout.Dimensions {
	paint.Fill(gtx.Ops, e.th.Background)
	if !e.login.Authenticated {
		return e.login.Layout(gtx, e.th)
	}

	mth := material.NewTheme()
	mth.Palette.Fg = e.th.Text
	mth.Palette.Bg = e.th.Surface

	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return layout.Inset{Top: unit.Dp(10), Left: unit.Dp(10), Right: unit.Dp(10), Bottom: unit.Dp(10)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
							layout.Rigid(material.H5(mth, "BobUI Go Runtime Surface").Layout),
							layout.Rigid(material.Body2(mth, "Live Gio frame loop with DemoSurface, interactive scroll, touch/swipe, and executable WebView runtime.").Layout),
						)
					})
				}),
				layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
					return layout.Inset{Left: unit.Dp(10), Right: unit.Dp(10), Bottom: unit.Dp(10)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
						return e.demo.Layout(gtx, e.th)
					})
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return e.shell.LayoutTaskbar(gtx, e.th)
				}),
			)
		}),
		layout.Stacked(func(gtx layout.Context) layout.Dimensions {
			return e.shell.LayoutStartMenu(gtx, e.th)
		}),
	)
}
