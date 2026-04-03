package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/io/key"
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
	time   float64
}

func NewEngine() *Engine {
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - Vision & History"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Aetheria)
	im := kernel.GetInputManager()
	undo := kernel.GetUndoStack()
	
	// Complex 3D Application
	rm := &widgets.Raymarcher{}
	testWin := &widgets.Window{ID: "win_3d", Title: "Vision Engine", Pos: f32.Pt(100, 100), Size: f32.Pt(400, 400)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case key.Event:
			if tag.Name == "Z" && tag.Modifiers.Contains(key.ModShortcut) && tag.State == key.Press {
				// Native Go Multi-User Undo Trigger
				undo.Undo("sys-admin")
			}
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			e.time += 0.016
			rm.Time = e.time
			rm.Size = testWin.Size

			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Window with 3D Vision Engine inside
			testWin.Layout(gtx, th)
			rm.Layout(gtx)

			// 2. Multi-Cursor Render
			for _, dev := range im.GetDevices() {
				// ... (cursor logic)
			}

			op.InvalidateOp{}.Add(gtx.Ops)
			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
