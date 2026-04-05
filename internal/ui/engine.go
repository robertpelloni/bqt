package ui

import (
	"fmt"
	"log"

	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	shell *widgets.Shell
	login *widgets.LoginView
	demo  *widgets.DemoSurface
}

func NewEngine() *Engine {
	return &Engine{
		shell: &widgets.Shell{},
		login: &widgets.LoginView{},
		demo:  widgets.NewDemoSurface(),
	}
}

// Run remains a compile-safe baseline while the Gio event loop is stabilized.
// It now initializes the demo composition surface and exercises the executable WebView bridge runtime.
func (e *Engine) Run() error {
	_ = theme.GetTheme(theme.Cyberpunk)
	log.Println("OmniUI Go: Engine initialized in safe baseline mode.")
	if e.demo != nil {
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
	return nil
}
