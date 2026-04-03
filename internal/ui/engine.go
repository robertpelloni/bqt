package ui

import (
	"log"
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
}

func NewEngine() *Engine {
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - Kernel Terminal"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	
	// Spawning Native Go Shell
	sh, err := kernel.NewShell()
	if err != nil { log.Fatal(err) }
	term := &widgets.Terminal{Shell: sh}

	testWin := &widgets.Window{ID: "win_term", Title: "Root Terminal", Pos: f32.Pt(100, 100), Size: f32.Pt(600, 400)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			testWin.Layout(gtx, th)
			term.Layout(gtx, th)

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
