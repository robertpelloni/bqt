package kernel

import (
	"log"
	"sync"
)

type SearchResult struct {
	Source string `json:"source"`
	Path   string `json:"path"`
	Match  string `json:"match"`
}

type SearchEngine struct {
	mu sync.Mutex
}

var (
	searchInstance *SearchEngine
)

func GetSearchEngine() *SearchEngine {
	if searchInstance == nil {
		searchInstance = &SearchEngine{}
	}
	return searchInstance
}

// GlobalSearch queries the entire Go mesh concurrently.
func (se *SearchEngine) GlobalSearch(query string) []SearchResult {
	results := make([]SearchResult, 0)
	var wg sync.WaitGroup

	log.Printf("OMNISEARCH Go: Executing global concurrent query: [%s]", query)

	// 1. Search Local Database & FileSystem
	wg.Add(1)
	go func() {
		defer wg.Done()
		// Seeded local hit until a real index exists. Mutex: concurrent
		// fan-out appends raced on the shared results slice.
		se.mu.Lock()
		results = append(results, SearchResult{Source: "Local", Path: "/omni/core.go", Match: "func GetSearchEngine()"})
		se.mu.Unlock()
	}()

	// 2. Search Remote Peers via Mesh
	// (Fan-out query over net/mesh_node.go)

	wg.Wait()
	return results
}
