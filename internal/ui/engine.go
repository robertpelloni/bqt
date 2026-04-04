package ui

import (
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
// It now initializes the demo composition surface and the lightweight WebView event hooks.
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
			log.Printf("BOBUI WEBVIEW: script message kind=%s channel=%s payload=%s", msg.Kind, msg.Channel, msg.Payload)
		}
		// demonstrate baseline bridge plumbing
		e.demo.WebView.PostMessage("boot", "demo-surface-online")
		e.demo.WebView.EvalJS("window.bobuiReady = true")
	}
	return nil
}
