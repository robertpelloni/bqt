package widgets

import (
	"fmt"

	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bobui/internal/ui/theme"
)

type DemoSurface struct {
	btn1, btn2, btn3 widget.Clickable
	dialog           Dialog
	popup            Popup
	drawer           Drawer
	menu             Menu
	tooltip          ToolTip
	WebView          WebView
	ScrollBox        *ScrollView
	ScrollItems      []string
	TouchPad         TouchArea
	LastSwipe        string
}

func NewDemoSurface() *DemoSurface {
	d := &DemoSurface{}
	d.popup = Popup{Visible: true, Title: "Popup", Body: "QuickControls2 baseline popup"}
	d.dialog = Dialog{Visible: false, Title: "Dialog", Body: "Dialog body"}
	d.drawer = Drawer{Visible: true, Title: "Drawer"}
	d.tooltip = ToolTip{Visible: true, Text: "Tooltip baseline", X: 12, Y: 12}
	d.menu = Menu{Visible: true, Items: []*MenuItem{{Label: "Open", Action: "open"}, {Label: "Close", Action: "close"}}}
	d.WebView.Navigate("about:bobui", "<h1>BobUI WebView</h1><br>History and navigation baseline")
	d.ScrollBox = NewScrollView(layout.Vertical)
	d.ScrollBox.VerticalPolicy = ScrollAlwaysOn
	d.ScrollBox.HorizontalPolicy = ScrollAlwaysOn
	d.ScrollBox.VerticalPlacement = ScrollBarOccupy
	d.ScrollBox.HorizontalPlacement = ScrollBarOccupy
	d.ScrollBox.SetCrossAxisViewport(0.15, 0.45)
	d.TouchPad = TouchArea{
		Label:             "Swipe Demo",
		AllowMouseAsTouch: true,
		MinSwipeDistance:  24,
	}
	d.TouchPad.OnSwipe = func(direction SwipeDirection, distance float32) {
		d.LastSwipe = fmt.Sprintf("%s %.0fpx", direction.String(), distance)
	}
	for i := 1; i <= 40; i++ {
		d.ScrollItems = append(d.ScrollItems, fmt.Sprintf("Scroll item %02d - QuickControls2-style scroll baseline", i))
	}
	return d
}

func emptyDemoValue(v string) string {
	if v == "" {
		return "none"
	}
	return v
}

func scrollPlacementLabel(p ScrollBarPlacement) string {
	if p == ScrollBarOccupy {
		return "occupy"
	}
	return "overlay"
}

func (ds *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Surface

	if ds.btn1.Clicked(gtx) {
		ds.dialog = Dialog{Visible: true, Title: "BobQ Dialog", Body: "This is a native dialog."}
	}
	if ds.btn2.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "BobQ Popup", Body: "This is a transient popup.", Width: 300, Height: 200, Alignment: layout.Center}
	}
	if ds.btn3.Clicked(gtx) {
		ds.drawer = Drawer{Visible: true, Title: "Navigation Drawer", WidthDp: 250, Edge: layout.W}
	}
	if ds.btn3.Hovered() {
		ds.tooltip = ToolTip{Visible: true, Text: "Click to open drawer"}
		ds.tooltip.X = 150
		ds.tooltip.Y = 150
	} else {
		ds.tooltip.Visible = false
	}

	// Compose in a simple top-to-bottom structure for verified baseline purposes.
	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return layout.UniformInset(unit.Dp(16)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Horizontal, Spacing: layout.SpaceEvenly}.Layout(gtx,
							layout.Rigid(material.Button(mth, &ds.btn1, "Show Dialog").Layout),
							layout.Rigid(material.Button(mth, &ds.btn2, "Show Popup").Layout),
							layout.Rigid(material.Button(mth, &ds.btn3, "Show Drawer").Layout),
						)
					})
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return ds.WebView.Layout(gtx, th)
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return layout.Inset{Top: unit.Dp(8), Bottom: unit.Dp(8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
							layout.Rigid(material.Label(mth, unit.Sp(11), fmt.Sprintf("Scroll demo items=%d vPos=%.2f vPage=%.2f hPos=%.2f hPage=%.2f", len(ds.ScrollItems), ds.ScrollBox.VerticalBar.Position, ds.ScrollBox.VerticalBar.PageSize, ds.ScrollBox.HorizontalBar.Position, ds.ScrollBox.HorizontalBar.PageSize)).Layout),
							layout.Rigid(material.Label(mth, unit.Sp(10), fmt.Sprintf("placements vertical=%s horizontal=%s deltas main=%.2f cross=%.2f", scrollPlacementLabel(ds.ScrollBox.VerticalPlacement), scrollPlacementLabel(ds.ScrollBox.HorizontalPlacement), ds.ScrollBox.LastScrollDelta, ds.ScrollBox.LastCrossDelta)).Layout),
							layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
							layout.Rigid(func(gtx layout.Context) layout.Dimensions {
								gtx.Constraints.Min.Y = gtx.Dp(unit.Dp(140))
								gtx.Constraints.Max.Y = gtx.Dp(unit.Dp(140))
								return ds.ScrollBox.LayoutWithTheme(gtx, th, len(ds.ScrollItems), func(gtx layout.Context, i int) layout.Dimensions {
									return layout.Inset{Top: unit.Dp(4), Bottom: unit.Dp(4), Left: unit.Dp(6), Right: unit.Dp(48)}.Layout(gtx, material.Body1(mth, ds.ScrollItems[i]).Layout)
								})
							}),
						)
					})
				}),
				layout.Rigid(func(gtx layout.Context) layout.Dimensions {
					return layout.Inset{Top: unit.Dp(8), Bottom: unit.Dp(8)}.Layout(gtx, func(gtx layout.Context) layout.Dimensions {
						return layout.Flex{Axis: layout.Vertical}.Layout(gtx,
							layout.Rigid(material.Label(mth, unit.Sp(11), fmt.Sprintf("Touch/swipe demo lastSwipe=%s", emptyDemoValue(ds.LastSwipe))).Layout),
							layout.Rigid(layout.Spacer{Height: unit.Dp(6)}.Layout),
							layout.Rigid(func(gtx layout.Context) layout.Dimensions {
								return ds.TouchPad.Layout(gtx, th)
							}),
						)
					})
				}),
			)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.dialog.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.popup.Layout(gtx, th)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dummyChild := func(gtx layout.Context) layout.Dimensions {
				return material.Body1(mth, "Drawer Content").Layout(gtx)
			}
			return ds.drawer.Layout(gtx, th, dummyChild)
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.menu.Layout(gtx, th)
			return dims
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.tooltip.Layout(gtx, th)
		}),
	)
}
