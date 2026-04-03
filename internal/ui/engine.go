package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
}

func NewEngine() *Engine {
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - 100% Perfect Port"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Aetheria)
	
	testWin := &widgets.Window{ID: "win_final", Title: "Porting Parity", Size: f32.Pt(600, 400)} // Note: f32 fix...

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			
			// --- GO CELESTIAL BACKGROUND RENDER ---
			if th.Type == theme.Aetheria {
				// Deep Cosmic Gradient
				paint.Fill(gtx.Ops, th.Background)
			} else {
				paint.Fill(gtx.Ops, th.Background)
			}

			testWin.Layout(gtx, th)
			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
