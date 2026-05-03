package data

import (
	"log"
	"os/exec"
)

type GitKernel struct {
	RepoPath string
}

func GetGitKernel() *GitKernel {
	return &GitKernel{RepoPath: "./"}
}

// AutonomousCommit creates a Git record of the OS evolution.
func (gk *GitKernel) AutonomousCommit(version string) error {
	// 1. Stage mutated Go source files
	exec.Command("git", "add", ".").Run()

	// 2. Commit the new DNA
	msg := "feat: autonomous kernel evolution to v" + version
	cmd := exec.Command("git", "commit", "-m", msg)
	if err := cmd.Run(); err != nil {
		return err
	}

	log.Printf("OMNIGENOME Go: Evolution physically committed to history: %s", msg)
	return nil
}

// BroadcastEvolution notifies the mesh that the DNA has changed.
func (gk *GitKernel) BroadcastEvolution(version string) {
	// Blast the new git hash over internal/net/mesh_node.go...
}
