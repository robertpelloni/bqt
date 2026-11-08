package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestCodeEditorLayout(t *testing.T) {
	editor := &CodeEditor{}
	editor.Editor.SetText("package main\n\nfunc main() {}\n")

	var ops op.Ops
	gtx := layout.Context{
		Ops: &ops,
		Constraints: layout.Exact(image.Point{
			X: 500,
			Y: 300,
		}),
		Metric: unit.Metric{PxPerDp: 1.0, PxPerSp: 1.0},
	}

	th := theme.GetTheme(theme.Dark)

	dims := editor.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions for CodeEditor, got %+v", dims.Size)
	}

	if editor.Editor.Text() != "package main\n\nfunc main() {}\n" {
		t.Fatalf("unexpected editor content: %q", editor.Editor.Text())
	}
}
