package widgets

import (
	"image"
	"math"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/op/clip"
	"gioui.org/op/paint"
	giowidget "gioui.org/widget"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type ScrollOrientation int

const (
	ScrollVertical ScrollOrientation = iota
	ScrollHorizontal
)

// ScrollBar is a lightweight QuickControls2-style scrollbar baseline.
// It now includes interactive click/drag behavior while preserving a small,
// compile-safe BobUI surface.
type ScrollBar struct {
	Orientation ScrollOrientation
	Position    float32 // 0..1
	PageSize    float32 // 0..1 visible fraction
	Visible     bool

	state        giowidget.Scrollbar
	Hovered      bool
	Dragging     bool
	TrackHovered bool
	LastDelta    float32
}

func (sb *ScrollBar) SetViewport(position, pageSize float32) {
	sb.Position = scrollClamp01(position)
	if pageSize <= 0 {
		pageSize = 0.2
	}
	sb.PageSize = scrollClamp01(pageSize)
	if sb.PageSize == 0 {
		sb.PageSize = 0.2
	}
}

func (sb *ScrollBar) ScrollDistance() float32 {
	return sb.LastDelta
}

func (sb *ScrollBar) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	if !sb.Visible {
		sb.LastDelta = 0
		sb.Hovered = false
		sb.TrackHovered = false
		sb.Dragging = false
		return layout.Dimensions{}
	}

	sb.SetViewport(sb.Position, sb.PageSize)
	axis := layout.Vertical
	if sb.Orientation == ScrollHorizontal {
		axis = layout.Horizontal
	}
	viewportStart := sb.Position
	viewportEnd := scrollClamp01(sb.Position + sb.PageSize)
	if viewportEnd < viewportStart {
		viewportEnd = viewportStart
	}

	sb.state.Update(gtx, axis, viewportStart, viewportEnd)
	sb.LastDelta = sb.state.ScrollDistance()
	sb.Dragging = sb.state.Dragging()
	sb.TrackHovered = sb.state.TrackHovered()
	sb.Hovered = sb.TrackHovered || sb.state.IndicatorHovered()

	track := image.Rect(0, 0, gtx.Constraints.Max.X, gtx.Constraints.Max.Y)
	if track.Dx() <= 0 || track.Dy() <= 0 {
		return layout.Dimensions{}
	}

    // Draw track
    trackColor := th.Surface
    trackColor.A = 100 // Semi transparent track
	paint.FillShape(gtx.Ops, trackColor, clip.Rect(track).Op())

    // Add interaction to track
    defer clip.Rect(track).Push(gtx.Ops).Pop()
	sb.state.AddTrack(gtx.Ops)

    // Calculate Thumb Dimensions
	var thumb image.Rectangle
    var thumbColor = th.Primary
	if sb.Hovered || sb.Dragging {
		thumbColor = th.Accent
	}

	if sb.Orientation == ScrollVertical {
		h := track.Dy()
		thumbH := scrollMaxInt(8, int(float32(h)*sb.PageSize))
		maxY := scrollMaxInt(0, h-thumbH)
		y := int(float32(maxY) * sb.Position)

        // Make the thumb slightly thinner than the track
        padding := gtx.Dp(2)
        thumbW := track.Dx() - padding*2
        if thumbW < 2 { thumbW = 2 }

		thumb = image.Rect(padding, y, padding+thumbW, y+thumbH)
	} else {
		w := track.Dx()
		thumbW := scrollMaxInt(8, int(float32(w)*sb.PageSize))
		maxX := scrollMaxInt(0, w-thumbW)
		x := int(float32(maxX) * sb.Position)

        padding := gtx.Dp(2)
        thumbH := track.Dy() - padding*2
        if thumbH < 2 { thumbH = 2 }

		thumb = image.Rect(x, padding, x+thumbW, padding+thumbH)
	}

    // Draw Thumb
    defer op.Offset(thumb.Min).Push(gtx.Ops).Pop()
	thumbLocal := image.Rect(0, 0, thumb.Dx(), thumb.Dy())

    // Rounded corners for the thumb
    radius := float32(gtx.Dp(4))
    if float32(thumbLocal.Dx())/2 < radius { radius = float32(thumbLocal.Dx())/2 }
    if float32(thumbLocal.Dy())/2 < radius { radius = float32(thumbLocal.Dy())/2 }

    rr := clip.UniformRRect(thumbLocal, int(math.Round(float64(radius))))
    paint.FillShape(gtx.Ops, thumbColor, rr.Op(gtx.Ops))

    // Add interaction to thumb
    defer rr.Push(gtx.Ops).Pop()
	sb.state.AddIndicator(gtx.Ops)

	return layout.Dimensions{Size: track.Size()}
}

func scrollClamp01(v float32) float32 {
	if v < 0 {
		return 0
	}
	if v > 1 {
		return 1
	}
	return v
}

func scrollMaxInt(a, b int) int {
	if a > b {
		return a
	}
	return b
}
