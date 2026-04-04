package ui

import (
	"log"

	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	shell  *widgets.Shell
	login  *widgets.LoginView
	rewind *widgets.RewindSlider
}

func NewEngine() *Engine {
	return &Engine{
		shell:  &widgets.Shell{},
		login:  &widgets.LoginView{},
		rewind: &widgets.RewindSlider{Active: true},
	}
}

// Run is currently a compile-safe baseline while the Gio event loop is stabilized.
// It intentionally avoids stale API assumptions and simply initializes core UI structs.
func (e *Engine) Run() error {
	_ = theme.GetTheme(theme.Cyberpunk)
	log.Println("OmniUI Go: Engine initialized in safe baseline mode.")
	return nil
}
