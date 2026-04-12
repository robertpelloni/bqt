package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
)

// BobQBox achieves GTK GtkBox functional parity in the pure Go UI layer.
// It wraps Gio's layout.Flex to provide sequential packing.
type BobQBox struct {
	Orientation layout.Axis
	Spacing     int
	Homogeneous bool
	children    []boxChild
}

type boxChild struct {
	widget  layout.Widget
	expand  bool
	fill    bool
	padding int
	packEnd bool
}

// NewBobQBox initializes a new box layout matching GTK semantics.
func NewBobQBox(orientation layout.Axis, spacing int, homogeneous bool) *BobQBox {
	return &BobQBox{
		Orientation: orientation,
		Spacing:     spacing,
		Homogeneous: homogeneous,
	}
}

// PackStart adds a widget to the box, packed from the start (left/top).
func (b *BobQBox) PackStart(w layout.Widget, expand, fill bool, padding int) {
	b.children = append(b.children, boxChild{
		widget:  w,
		expand:  expand,
		fill:    fill,
		padding: padding,
		packEnd: false,
	})
}

// PackEnd adds a widget to the box, packed from the end (right/bottom).
func (b *BobQBox) PackEnd(w layout.Widget, expand, fill bool, padding int) {
	b.children = append(b.children, boxChild{
		widget:  w,
		expand:  expand,
		fill:    fill,
		padding: padding,
		packEnd: true,
	})
}

// Layout renders the children according to GtkBox semantics.
func (b *BobQBox) Layout(gtx layout.Context) layout.Dimensions {
	var startFlex []layout.FlexChild
	var endFlex []layout.FlexChild

	for _, child := range b.children {
		var flexChild layout.FlexChild

		weight := float32(0)
		if b.Homogeneous || child.expand {
			weight = 1
		}

		flexChild = layout.Flexed(weight, func(gtx layout.Context) layout.Dimensions {
			return layout.Inset{
				Top:    unit.Dp(child.padding),
				Bottom: unit.Dp(child.padding),
				Left:   unit.Dp(child.padding),
				Right:  unit.Dp(child.padding),
			}.Layout(gtx, child.widget)
		})

		if child.packEnd {
			endFlex = append([]layout.FlexChild{flexChild}, endFlex...)
		} else {
			startFlex = append(startFlex, flexChild)
		}
	}

	allChildren := startFlex
	if len(endFlex) > 0 {
		spacer := layout.Flexed(1, func(gtx layout.Context) layout.Dimensions {
			return layout.Dimensions{}
		})
		allChildren = append(allChildren, spacer)
		allChildren = append(allChildren, endFlex...)
	}

	flex := layout.Flex{
		Axis:      b.Orientation,
		Spacing:   layout.SpaceEnd,
		Alignment: layout.Start,
	}

	return flex.Layout(gtx, allChildren...)
}
