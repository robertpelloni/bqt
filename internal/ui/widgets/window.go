package widgets

import (
	"image"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"gioui.org/f32"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	Title    string
	Pos      f32.Point
	Size     f32.Point
	Dragging bool
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	// --- CYBERPUNK GEOMETRY PASS (Go Port) ---
	// Replacing C++ QPainterPath with Go clip.Path
	var path clip.Path
	path.Begin(gtx.Ops)
	
	rect := f32.Rect(0, 0, w.Size.X, w.Size.Y)
	cut := float32(20.0)

	path.MoveTo(f32.Pt(cut, 0))
	path.LineTo(f32.Pt(w.Size.X, 0))
	path.LineTo(f32.Pt(w.Size.X, w.Size.Y-cut))
	path.LineTo(f32.Pt(w.Size.X-cut, w.Size.Y))
	path.LineTo(f32.Pt(0, w.Size.Y))
	path.LineTo(f32.Pt(0, cut))
	path.Close()

	// Fill Surface
	stack := clip.Outline{Path: path.End()}.Op().Push(gtx.Ops)
	paint.ColorOp{Color: th.Surface}.Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)
	
	// Draw Neon Border
	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: path.End(), Width: 2}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)
	
	stack.Pop()

	return layout.Dimensions{Size: image.Pt(int(w.Size.X), int(w.Size.Y))}
}
