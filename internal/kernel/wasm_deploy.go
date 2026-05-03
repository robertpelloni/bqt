package kernel

import (
	"log"
	"os/exec"
	"net/http"
)

type WASMDeployer struct {
	WebRoot string
}

func GetWASMDeployer() *WASMDeployer {
	return &WASMDeployer{WebRoot: "./dist"}
}

// SelfReplicate triggers a local Go compilation to WASM for browser propagation.
func (wd *WASMDeployer) SelfReplicate() error {
	log.Println("OMNIWASM Go: Initiating self-replication sequence...")
	
	cmd := exec.Command("go", "build", "-o", wd.WebRoot+"/kernel.wasm", "main.go")
	cmd.Env = append(cmd.Env, "GOOS=js", "GOARCH=wasm")
	
	if err := cmd.Run(); err != nil {
		return err
	}

	log.Printf("OMNIWASM Go: Replication successful. Browser payload ready at %s", wd.WebRoot)
	return nil
}

// ServeChildOS starts a local web server to host the replicated WASM kernel.
func (wd *WASMDeployer) ServeChildOS(port string) {
	go func() {
		log.Printf("OMNIWASM Go: Child OS hosting active on port %s", port)
		http.ListenAndServe(":"+port, http.FileServer(http.Dir(wd.WebRoot)))
	}()
}
