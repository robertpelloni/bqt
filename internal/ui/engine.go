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
// It now initializes the demo composition surface so the new controls exist in one place.
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
	}
	return nil
}
