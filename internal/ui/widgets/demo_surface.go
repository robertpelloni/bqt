package widgets

import (
	"gioui.org/layout"
	"gioui.org/unit"
	"gioui.org/widget"
	"gioui.org/widget/material"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

type DemoSurface struct {
	WebView          WebView
	btn1, btn2, btn3 widget.Clickable
    dialog           Dialog
    popup            Popup
    drawer           Drawer
    menu             Menu
    tooltip          ToolTip
}

func (ds *DemoSurface) Layout(gtx layout.Context, th theme.Theme) layout.Dimensions {
	mth := material.NewTheme()
	mth.Palette.Fg = th.Text
	mth.Palette.Bg = th.Primary

	if ds.btn1.Clicked(gtx) {
		ds.dialog = Dialog{Visible: true, Title: "BobQ Dialog", Body: "This is a native dialog."}
	}
    if ds.btn2.Clicked(gtx) {
		ds.popup = Popup{Visible: true, Title: "BobQ Popup", Body: "This is a transient popup."}
	}
    if ds.btn3.Clicked(gtx) {
		ds.drawer = Drawer{Visible: true, Title: "Navigation Drawer", WidthDp: 250}
	}
    if ds.btn3.Hovered() {
        ds.tooltip = ToolTip{Visible: true, Text: "Click to open drawer"}
        ds.tooltip.X = 150
        ds.tooltip.Y = 150
    } else {
        ds.tooltip.Visible = false
    }

    // Init menu items if needed
    if len(ds.menu.Items) == 0 {
        ds.menu.Items = []*MenuItem{
            {Label: "File", Action: "file"},
            {Label: "Edit", Action: "edit"},
        }
    }

	return layout.Stack{}.Layout(gtx,
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return layout.UniformInset(unit.Dp(16)).Layout(gtx, func(gtx layout.Context) layout.Dimensions {
				return layout.Flex{Axis: layout.Vertical, Spacing: layout.SpaceEvenly}.Layout(gtx,
					layout.Rigid(material.Button(mth, &ds.btn1, "Show Dialog").Layout),
                    layout.Rigid(material.Button(mth, &ds.btn2, "Show Popup").Layout),
                    layout.Rigid(material.Button(mth, &ds.btn3, "Show Drawer").Layout),
				)
			})
		}),
		layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			dims, _ := ds.dialog.Layout(gtx, th)
            return dims
		}),
        layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			return ds.popup.Layout(gtx, th)
		}),
        layout.Expanded(func(gtx layout.Context) layout.Dimensions {
			// dummy child for drawer
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

func NewDemoSurface() *DemoSurface {
	return &DemoSurface{}
}
