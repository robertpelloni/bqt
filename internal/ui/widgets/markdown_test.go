package widgets

import (
	"image"
	"testing"

	"gioui.org/layout"
	"gioui.org/op"
	"gioui.org/unit"
	"github.com/robertpelloni/bqt/internal/ui/theme"
)

func TestMarkdownViewLayout(t *testing.T) {
	mv := &MarkdownView{
		Text: "# Heading 1\n## Heading 2\nRegular paragraph line.\nAnother line.",
	}

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

	dims := mv.Layout(gtx, th)
	if dims.Size.X <= 0 || dims.Size.Y <= 0 {
		t.Fatalf("expected non-zero layout dimensions for populated markdown, got %+v", dims.Size)
	}

	// Empty markdown text
	mvEmpty := &MarkdownView{Text: ""}
	dimsEmpty := mvEmpty.Layout(gtx, th)
	if dimsEmpty.Size.X < 0 || dimsEmpty.Size.Y < 0 {
		t.Fatalf("expected valid layout dimensions for empty markdown, got %+v", dimsEmpty.Size)
	}
}
