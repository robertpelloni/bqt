package ui

import (
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
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Secure IDE")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	// Application Widgets
	editor := &widgets.CodeEditor{}
	testWin := &widgets.Window{ID: "win_1", Title: "Main Editor", Pos: f32.Pt(50, 50), Size: f32.Pt(800, 500)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// Render Interactive Window with Permissioning
			testWin.Layout(gtx, th)
			
			// Render Editor inside window context
			// ... (clipping logic)
			editor.Layout(gtx, th)

			// Multi-Cursor Overlay
			for _, dev := range im.GetDevices() {
				// ... (rendering logic)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
