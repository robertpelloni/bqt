package widgets

import "gioui.org/layout"

type ScrollPolicy int

const (
	ScrollAsNeeded ScrollPolicy = iota
	ScrollAlwaysOn
	ScrollAlwaysOff
)

// ScrollView is a minimal QuickControls2-style scroll container baseline.
type ScrollView struct {
	List               layout.List
	VerticalPolicy     ScrollPolicy
	HorizontalPolicy   ScrollPolicy
	VerticalBar        ScrollBar
	HorizontalBar      ScrollBar
	ApproxContentItems int
}

func NewScrollView(axis layout.Axis) *ScrollView {
	return &ScrollView{
		List:             layout.List{Axis: axis},
		VerticalPolicy:   ScrollAsNeeded,
		HorizontalPolicy: ScrollAlwaysOff,
		VerticalBar:      ScrollBar{Orientation: ScrollVertical},
		HorizontalBar:    ScrollBar{Orientation: ScrollHorizontal},
	}
}

func (sv *ScrollView) Layout(gtx layout.Context, count int, child func(layout.Context, int) layout.Dimensions) layout.Dimensions {
	sv.ApproxContentItems = count
	return sv.List.Layout(gtx, count, child)
}
