package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"gioui.org/io/pointer"
	"github.com/robertpelloni/bobui/internal/kernel"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	ID       string
	Title    string
	Pos      f32.Point
	Size     f32.Point
	Dragging bool
	DragID   pointer.ID
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	pm := kernel.GetPermissionManager()
	currentDevice := "sys-mouse-0" 

	for _, ev := range gtx.Events(w) {
		if e, ok := ev.(pointer.Event); ok {
			switch e.Type {
			case pointer.Press:
				if pm.TryLock(currentDevice, w.ID) { w.Dragging = true; w.DragID = e.PointerID }
			case pointer.Release:
				w.Dragging = false; pm.Unlock(currentDevice, w.ID)
			case pointer.Move:
				if w.Dragging && pm.CanInteract(currentDevice, w.ID) { w.Pos = w.Pos.Add(e.Position) }
			}
		}
	}

	defer op.Offset(image.Pt(int(w.Pos.X), int(w.Pos.Y))).Push(gtx.Ops).Pop()
	rect := f32.Rect(0, 0, w.Size.X, w.Size.Y)
	
	// --- GO HIGH-ART RENDER PASS (100% Parity) ---
	if th.Type == theme.Cyberpunk {
		// Draw Neon Trace Circuit Lines Natively in Go
		for i := float32(40); i < w.Size.Y; i += 40 {
			var p clip.Path
			p.Begin(gtx.Ops)
			p.MoveTo(f32.Pt(0, i))
			p.LineTo(f32.Pt(w.Size.X, i))
			paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
			clip.Stroke{Path: p.End(), Width: 0.5}.Op().Add(gtx.Ops)
			paint.PaintOp{}.Add(gtx.Ops)
		}
	}

	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect.Round()).Op())
	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: clip.Rect(rect.Round()).Path(), Width: 1.5}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: rect.Round().Size()}
}
