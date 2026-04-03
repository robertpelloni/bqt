package widgets

import (
	"image"
	"math"
	"sync"
	"gioui.org/layout"
	"gioui.org/op/paint"
	"gioui.org/f32"
)

type Raymarcher struct {
	Time float64
	Size f32.Point
}

func (r *Raymarcher) Layout(gtx layout.Context) layout.Dimensions {
	w, h := int(r.Size.X)/4, int(r.Size.Y)/4 // Low-res buffer for CPU speed
	if w <= 0 || h <= 0 { return layout.Dimensions{} }

	img := image.NewRGBA(image.Rect(0, 0, w, h))
	
	// --- GO CONCURRENT 3D SOLVER ---
	var wg sync.WaitGroup
	for y := 0; y < h; y++ {
		wg.Add(1)
		go func(y int) {
			defer wg.Done()
			for x := 0; x < w; x++ {
				// Normalized Camera Ray Calculation
				uvX := (2.0*float64(x) - float64(w)) / float64(h)
				uvY := (2.0*float64(y) - float64(h)) / float64(h)
				
				// Ported Sphere SDF Logic
				dist := math.Sqrt(uvX*uvX + uvY*uvY) - (0.5 + 0.1*math.Sin(r.Time))
				if dist < 0 {
					img.Set(x, y, image.RGBAColor{R: 212, G: 175, B: 55, A: 255}) // Aetheria Gold
				} else {
					img.Set(x, y, image.RGBAColor{R: 10, G: 2, B: 20, A: 255})    // Nebula Purple
				}
			}
		}(y)
	}
	wg.Wait()

	paint.NewImageOp(img).Add(gtx.Ops)
	paint.PaintOp{}.Add(gtx.Ops)

	return layout.Dimensions{Size: gtx.Constraints.Max}
}
