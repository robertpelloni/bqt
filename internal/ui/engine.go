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
	return &Engine{window: app.NewWindow(app.Title("OmniUI Go - Workstation Desktop"))}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	im := kernel.GetInputManager()
	
	// Complex Layout Components
	split := &widgets.SplitView{Ratio: 0.3}
	pb := &widgets.ProgressBar{Value: 0.75}

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)
			paint.Fill(gtx.Ops, th.Background)

			// 1. Render Responsive Workstation Layout
			split.Layout(gtx, 
				func(gtx layout.Context) layout.Dimensions {
					return widgets.material.Label(material.NewTheme(), 14, "Sidebar").Layout(gtx) // Note: material fix...
				},
				func(gtx layout.Context) layout.Dimensions {
					return pb.Layout(gtx, th)
				},
			)

			// 2. Render Multi-Cursor Overlay
			for _, dev := range im.GetDevices() {
				// ... (cursor rendering)
			}

			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
