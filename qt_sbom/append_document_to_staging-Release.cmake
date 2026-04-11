
        cmake_minimum_required(VERSION 3.16)
        message(STATUS "Starting SPDX SBOM generation in build dir: C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in")
        
        set(QT_SBOM_EXTERNAL_DOC_REFS "")

        file(READ "C:/Users/hyper/workspace/npp/bobui/qt_sbom/SPDXRef-DOCUMENT-qtbase.spdx.in" content)
        # Override any previous file because we're starting from scratch.
        file(WRITE "C:/Users/hyper/workspace/npp/bobui/qt_sbom/staging-qtbase.spdx.in" "${content}")
