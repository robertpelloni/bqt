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
	Grabbed  bool
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	rect := f32.Rect(w.Pos.X, w.Pos.Y, w.Pos.X+w.Size.X, w.Pos.Y+w.Size.Y)

	// 1. Render Grab Glow (Neon Bloom)
	if w.Grabbed {
		DrawGrabGlow(gtx, rect, th.Primary)
	}

	// 2. Render Window Body
	var path clip.Path
	path.Begin(gtx.Ops)
	cut := float32(20.0)
	
	path.MoveTo(f32.Pt(w.Pos.X+cut, w.Pos.Y))
	path.LineTo(f32.Pt(w.Pos.X+w.Size.X, w.Pos.Y))
	path.LineTo(f32.Pt(w.Pos.X+w.Size.X, w.Pos.Y+w.Size.Y-cut))
	path.LineTo(f32.Pt(w.Pos.X+w.Size.X-cut, w.Pos.Y+w.Size.Y))
	path.LineTo(f32.Pt(w.Pos.X, w.Pos.Y+w.Size.Y))
	path.LineTo(f32.Pt(w.Pos.X, w.Pos.Y+cut))
	path.Close()

	stack := clip.Outline{Path: path.End()}.Op().Push(gtx.Ops)
	paint.Fill(gtx.Ops, th.Surface)
	stack.Pop()

	// 3. Render Border
	paint.ColorOp{Color: th.Primary}.Add(gtx.Ops)
	clip.Stroke{Path: path.End(), Width: 1.5}.Op().Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: rect.Round().Size()}
}
