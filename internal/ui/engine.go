package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/widgets"
)

type Engine struct {
	window *app.Window
	shell  *widgets.Shell
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Perfect Shell")),
		shell:  &widgets.Shell{},
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	
	// Final UI Suite
	cal := &widgets.Calendar{}
	testWin := &widgets.Window{ID: "win_cal", Title: "Temporal Ledger", Pos: f32.Pt(50, 50), Size: f32.Pt(300, 350)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Desktop Area
			testWin.Layout(gtx, th)
			cal.Layout(gtx, th)

			// 2. Render Polished Taskbar
			e.shell.LayoutTaskbar(gtx, th)

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
