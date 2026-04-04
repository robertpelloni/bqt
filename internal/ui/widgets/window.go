package widgets

import (
	"image"

	"gioui.org/f32"
	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type Window struct {
	ID    string
	Title string
	Pos   f32.Point
	Size  f32.Point
}

func (w *Window) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	rect := image.Rect(int(w.Pos.X), int(w.Pos.Y), int(w.Pos.X+w.Size.X), int(w.Pos.Y+w.Size.Y))
	titleBar := image.Rect(rect.Min.X, rect.Min.Y, rect.Max.X, rect.Min.Y+30)

	defer op.Offset(image.Pt(0, 0)).Push(gtx.Ops).Pop()
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(rect).Op())
	paint.FillShape(gtx.Ops, th.Primary, clip.Rect(titleBar).Op())
	if th.Type == theme.Cyberpunk {
		for i := rect.Min.Y + 40; i < rect.Max.Y; i += 40 {
			line := image.Rect(rect.Min.X, i, rect.Max.X, i+1)
			paint.FillShape(gtx.Ops, th.Primary, clip.Rect(line).Op())
		}
	}
	return layout.Dimensions{Size: image.Pt(int(w.Size.X), int(w.Size.Y))}
}
