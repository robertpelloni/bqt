package ui

import (
	"gioui.org/app"
	"gioui.org/io/system"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bobui/internal/ui/theme"
	"github.com/robertpelloni/bobui/internal/ui/anim"
	"github.com/robertpelloni/bobui/internal/audio"
)

type Engine struct {
	window *app.Window
}

func NewEngine() *Engine {
	return &Engine{
		window: app.NewWindow(app.Title("OmniUI Go - Temporal Sync")),
	}
}

func (e *Engine) Run() error {
	var ops op.Ops
	th := theme.GetTheme(theme.Cyberpunk)
	clock := audio.GetMasterClock()
	animator := anim.GetAnimator()

	for event := range e.window.Events() {
		switch tag := event.(type) {
		case system.FrameEvent:
			gtx := layout.NewContext(&ops, tag)

			// 1. Process Temporal Sync (Go Port)
			select {
			case <-clock.TickChan:
				// Pulse UI elements on the beat
			default:
			}

			// 2. Process High-Speed Go Animations
			animator.Process()

			// 3. Background
			paint.Fill(gtx.Ops, th.Background)

			op.InvalidateOp{}.Add(gtx.Ops)
			tag.Frame(gtx.Ops)
		case system.DestroyEvent:
			return tag.Err
		}
	}
	return nil
}
