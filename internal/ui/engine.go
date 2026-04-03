package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
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
		window: app.NewWindow(app.Title("OmniUI Go - Distributed Desktop")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	shell := &widgets.Shell{}
	testWin := &widgets.Window{Title: "Root Console", Pos: f32.Pt(50, 50), Size: f32.Pt(600, 400)}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)

			// 1. Background
			paint.Fill(gtx.Ops, th.Background)

			// 2. Render Shell Taskbar
			shell.LayoutTaskbar(gtx, th)

			// 3. Render Windows with Ownership Glow
			testWin.Grabbed = true // Simulation
			testWin.Layout(gtx, th)

			// 4. Multi-Cursor Render
			for _, dev := range im.GetDevices() {
				var p clip.Path
				p.Begin(gtx.Ops)
				p.MoveTo(f32.Pt(float32(dev.X), float32(dev.Y)))
				p.LineTo(f32.Pt(float32(dev.X+15), float32(dev.Y+10)))
				p.LineTo(f32.Pt(float32(dev.X+10), float32(dev.Y+15)))
				p.Close()
				
				stack := clip.Outline{Path: p.End()}.Op().Push(gtx.Ops)
				paint.ColorOp{Color: th.Text}.Add(gtx.Ops)
				paint.PaintOp{}.Add(gtx.Ops)
				stack.Pop()
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
