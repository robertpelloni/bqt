package widgets

import (
	"math"

	"gioui.org/layout"
	giowidget "gioui.org/widget"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type ScrollPolicy int

const (
	ScrollAsNeeded ScrollPolicy = iota
	ScrollAlwaysOn
	ScrollAlwaysOff
)

// ScrollView is a minimal QuickControls2-style scroll container baseline.
// It now couples a Gio list position to BobUI scrollbar state so pointer
// interaction can change the viewport in a compile-safe way.
type ScrollView struct {
	List               giowidget.List
	VerticalPolicy     ScrollPolicy
	HorizontalPolicy   ScrollPolicy
	VerticalBar        ScrollBar
	HorizontalBar      ScrollBar
	ApproxContentItems int
	LastScrollDelta    float32
}

func NewScrollView(axis layout.Axis) *ScrollView {
	return &ScrollView{
		List:             giowidget.List{List: layout.List{Axis: axis}},
		VerticalPolicy:   ScrollAsNeeded,
		HorizontalPolicy: ScrollAlwaysOff,
		VerticalBar:      ScrollBar{Orientation: ScrollVertical},
		HorizontalBar:    ScrollBar{Orientation: ScrollHorizontal},
	}
}

func (sv *ScrollView) Layout(gtx layout.Context, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	return sv.layoutWithTheme(gtx, theme.GetTheme(theme.Cyberpunk), count, child)
}

func (sv *ScrollView) LayoutWithTheme(gtx layout.Context, th theme.Theme, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	return sv.layoutWithTheme(gtx, th, count, child)
}

func (sv *ScrollView) layoutWithTheme(gtx layout.Context, th theme.Theme, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	sv.ApproxContentItems = count
	listDims := sv.List.Layout(gtx, count, child)

	primary := sv.primaryBar()
	policy := sv.primaryPolicy()
	majorAxisSize := sv.List.Axis.Convert(listDims.Size).X
	start, end := scrollRangeFromListPosition(sv.List.Position, count, majorAxisSize)
	scrollable := scrollRangeIsScrollable(start, end)
	primary.Visible = scrollPolicyVisible(policy, scrollable)
	primary.SetViewport(start, end-start)

	gtx.Constraints.Min = listDims.Size
	if primary.Visible {
		anchor := layout.E
		if sv.List.Axis == layout.Horizontal {
			anchor = layout.S
		}
		anchor.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
			if sv.List.Axis == layout.Vertical {
				cross := gtx.Constraints.Max
				cross.X = gtx.Dp(12)
				gtx.Constraints.Min = cross
				gtx.Constraints.Max = cross
				return primary.Layout(gtx, th)
			}
			cross := gtx.Constraints.Max
			cross.Y = gtx.Dp(12)
			gtx.Constraints.Min = cross
			gtx.Constraints.Max = cross
			return primary.Layout(gtx, th)
		})
	}

	sv.LastScrollDelta = primary.ScrollDistance()
	if sv.LastScrollDelta != 0 && count > 0 {
		sv.List.ScrollBy(sv.LastScrollDelta * float32(count))
	}
	return listDims
}

func (sv *ScrollView) primaryBar() *ScrollBar {
	if sv.List.Axis == layout.Horizontal {
		return &sv.HorizontalBar
	}
	return &sv.VerticalBar
}

func (sv *ScrollView) primaryPolicy() ScrollPolicy {
	if sv.List.Axis == layout.Horizontal {
		return sv.HorizontalPolicy
	}
	return sv.VerticalPolicy
}

func scrollPolicyVisible(policy ScrollPolicy, scrollable bool) bool {
	switch policy {
	case ScrollAlwaysOn:
		return true
	case ScrollAlwaysOff:
		return false
	default:
		return scrollable
	}
}

func scrollRangeIsScrollable(start, end float32) bool {
	return end-start < 1
}

func scrollRangeFromListPosition(pos layout.Position, elements int, majorAxisSize int) (start, end float32) {
	if elements <= 0 || majorAxisSize <= 0 || pos.Length <= 0 {
		return 0, 1
	}
	lengthEstPx := float32(pos.Length)
	elementLenEstPx := lengthEstPx / float32(elements)
	listOffsetF := float32(pos.Offset)
	listOffsetL := float32(pos.OffsetLast)

	viewportStart := scrollClamp01((float32(pos.First)*elementLenEstPx + listOffsetF) / lengthEstPx)
	viewportEnd := scrollClamp01((float32(pos.First+pos.Count)*elementLenEstPx + listOffsetL) / lengthEstPx)
	viewportFraction := viewportEnd - viewportStart
	visiblePx := float32(majorAxisSize)
	visibleFraction := visiblePx / lengthEstPx
	err := visibleFraction - viewportFraction
	adjStart := viewportStart
	adjEnd := viewportEnd
	if viewportFraction < 1 {
		startShare := viewportStart / (1 - viewportFraction)
		endShare := (1 - viewportEnd) / (1 - viewportFraction)
		adjStart -= startShare * err
		adjEnd += endShare * err
	}
	adjStart = scrollClamp01(adjStart)
	adjEnd = scrollClamp01(adjEnd)
	if adjEnd < adjStart {
		adjEnd = adjStart
	}
	if math.Abs(float64(adjEnd-adjStart)) < 0.0001 {
		adjEnd = scrollClamp01(adjStart + 0.1)
	}
	return adjStart, adjEnd
}
