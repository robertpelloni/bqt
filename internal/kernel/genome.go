package kernel

import (
	"fmt"
	"log"
	"os"
	"github.com/robertpelloni/bobui/internal/vm"
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
	
	f, err := os.Create(path)
	if err != nil { return err }
	defer f.Close()

	_, err = f.WriteString(code)
	if err != nil { return err }

	log.Printf("OMNIGENOME Go: Kernel successfully evolved. New DNA at %s", path)
	
	// In a production build, this would trigger 'go build -buildmode=plugin' 
	// and load it via plugin_host.go instantly.
	return nil
}

// RequestEvolution asks the AI to propose a kernel-level improvement.
func (gk *GenomeKernel) RequestEvolution() {
	assistant := vm.GetAIAssistant()
	log.Println("OMNIGENOME Go: Requesting autonomous source-code evolution...")
	assistant.RequestCompletion("Propose a new Go-native UI component in Gio.")
}
