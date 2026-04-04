package widgets

import (
	"image"

	"gioui.org/layout"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type ScrollOrientation int

const (
	ScrollVertical ScrollOrientation = iota
	ScrollHorizontal
)

// ScrollBar is a lightweight QuickControls2-style scrollbar baseline.
type ScrollBar struct {
	Orientation ScrollOrientation
	Position    float32 // 0..1
	PageSize    float32 // 0..1 visible fraction
	Visible     bool
}

func (sb *ScrollBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !sb.Visible {
		return layout.Dimensions{}
	}
	if sb.PageSize <= 0 {
		sb.PageSize = 0.2
	}
	if sb.PageSize > 1 {
		sb.PageSize = 1
	}
	if sb.Position < 0 {
		sb.Position = 0
	}
	if sb.Position > 1 {
		sb.Position = 1
	}

	track := image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	paint.FillShape(gtx.Ops, th.Surface, clip.Rect(track).Op())

	if sb.Orientation == ScrollVertical {
		h := track.Dy()
		thumbH := scrollMaxInt(8, int(float32(h)*sb.PageSize))
		maxY := scrollMaxInt(0, h-thumbH)
		y := int(float32(maxY) * sb.Position)
		thumb := image.Rect(0, y, track.Dx(), y+thumbH)
		paint.FillShape(gtx.Ops, th.Primary, clip.Rect(thumb).Op())
	} else {
		w := track.Dx()
		thumbW := scrollMaxInt(8, int(float32(w)*sb.PageSize))
		maxX := scrollMaxInt(0, w-thumbW)
		x := int(float32(maxX) * sb.Position)
		thumb := image.Rect(x, 0, x+thumbW, track.Dy())
		paint.FillShape(gtx.Ops, th.Primary, clip.Rect(thumb).Op())
	}

	return layout.Dimensions{Size: track.Size()}
}

func scrollMaxInt(a, b int) int {
	if a > b {
		return a
	}
	return b
}
