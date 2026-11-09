package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestNotebook(t *testing.T) {
	notebook := &BQtNotebook{}

	page1Rendered := false
	page1Layout := func(gtx layout.Context) layout.Dimensions {
		page1Rendered = true
		return layout.Dimensions{Size: image.Pt(200, 100)}
	}
	notebook.AddPage("Page 1", page1Layout)

	page2Rendered := false
	page2Layout := func(gtx layout.Context) layout.Dimensions {
		page2Rendered = true
		return layout.Dimensions{Size: image.Pt(200, 100)}
	}
	notebook.AddPage("Page 2", page2Layout)

	if len(notebook.Pages) != 2 {
		t.Fatalf("Expected 2 pages, got %d", len(notebook.Pages))
	}

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 500,
			Y: 400,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	th := theme.GetTheme(theme.Dark)

	// Layout initial state (ActiveIndex = 0)
	dims := notebook.Layout(gtx, th)
	if !page1Rendered {
		t.Fatal("expected page 1 to render when ActiveIndex is 0")
	}
	if page2Rendered {
		t.Fatal("expected page 2 not to render when ActiveIndex is 0")
	}
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions, got %+v", dims.Size)
	}

	// Click tab index 1 to switch to Page 2
	page1Rendered = false
	notebook.tabs[1].Click()
	notebook.Layout(gtx, th)

	if notebook.ActiveIndex != 1 {
		t.Fatalf("expected ActiveIndex 1 after tab 1 click, got %d", notebook.ActiveIndex)
	}
	if !page2Rendered {
		t.Fatal("expected page 2 to render when ActiveIndex is 1")
	}
	if page1Rendered {
		t.Fatal("expected page 1 not to render when ActiveIndex is 1")
	}

	// Empty notebook layout returns empty dimensions
	emptyNotebook := &BQtNotebook{}
	dimsEmpty := emptyNotebook.Layout(gtx, th)
	if dimsEmpty.Size.X != 0 || dimsEmpty.Size.Y != 0 {
		t.Fatalf("expected 0x0 for empty notebook, got %+v", dimsEmpty.Size)
	}
}

func TestNotebookActiveIndexOutOfBounds(t *testing.T) {
	notebook := &BQtNotebook{}
	notebook.AddPage("Page 1", func(gtx layout.Context) layout.Dimensions {
		return layout.Dimensions{Size: image.Pt(100, 50)}
	})

	var ops op.Ops
	gtx := layout.Context{
		Ops:         &ops,
		Constraints: layout.Exact(image.Point{X: 300, Y: 200}),
		Metric:      unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}
	th := theme.GetTheme(theme.Dark)

	// Test negative index
	notebook.ActiveIndex = -1
	dims := notebook.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions with tab bar rendered, got %+v", dims.Size)
	}

	// Test index past page length
	notebook.ActiveIndex = 99
	dims = notebook.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero dimensions with tab bar rendered, got %+v", dims.Size)
	}
}
