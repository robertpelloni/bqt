package kernel

import (
	"fmt"
	"log"
	"os"
)

type GenomeKernel struct {
	EvolutionPath string
}

func GetGenomeKernel() *GenomeKernel {
	return &GenomeKernel{EvolutionPath: "./internal/evolution"}
}

// MutateSource physically writes new Go code to the system directory.
func (gk *GenomeKernel) MutateSource(packageName, code string) error {
	path := fmt.Sprintf("%s/%s.go", gk.EvolutionPath, packageName)

	if err := os.MkdirAll(gk.EvolutionPath, 0o755); err != nil {
		return err
	}

	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	_, err = f.WriteString(code)
	if err != nil {
		return err
	}

	log.Printf("OMNIGENOME Go: Kernel wrote new source to %s", path)
	return nil
}

// RequestEvolution is intentionally a lightweight stub in the verified Go baseline.
// The higher-level AI assistant can call MutateSource after generating code, but this
// package does not import the VM layer in order to avoid import cycles.
func (gk *GenomeKernel) RequestEvolution() {
	log.Println("OMNIGENOME Go: Evolution request queued for external AI orchestration.")
}
