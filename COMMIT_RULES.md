# Conventional Commit Guidelines for TermAMP

## Commit Format
Each commit message consists of a **header**, a **body**, and a **footer**. The header has a special format that includes a **type**, a **scope**, and a **subject**:

```
<type>(<scope>): <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

## Type
Must be one of the following:

- `feat`: A new feature (adds capability)
- `fix`: A bug fix (resolves an issue)
- `docs`: Documentation only changes
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
- `refactor`: A code change that neither fixes a bug nor adds a feature
- `perf`: A code change that improves performance
- `test`: Adding missing tests or correcting existing tests
- `build`: Changes that affect the build system or external dependencies (example scopes: gulp, broccoli, npm)
- `ci`: Changes to CI configuration files and scripts (example scopes: Travis, Circle, BrowserStack, SauceLabs)
- `chore`: Other changes that don't modify src or test files
- `revert`: Reverts a previous commit

## Scope
The scope should be a single word that provides additional contextual information. Examples for TermAMP:
- `ui`: User interface related changes
- `player`: Audio player functionality
- `playlist`: Playlist management
- `visualizer`: Audio visualization
- `gstreamer`: GStreamer integration
- `gtk`: GTK UI framework
- `build`: Build system
- `deps`: Dependencies
- `config`: Configuration files

## Subject
The subject contains a succinct description of the change:

- Use the imperative, present tense: "change" not "changed" nor "changes"
- Don't capitalize the first letter
- No dot (.) at the end

## Body
Just as in the **subject**, use the imperative, present tense: "change" not "changed" nor "changes".
The body should include the motivation for the change and contrast this with previous behavior.

## Footer
The footer should contain any information about **Breaking Changes** and is also the place to reference GitHub issues that this commit addresses.

## Examples

### Feature
```
feat(ui): add mini mode toggle button

Add a button to toggle between mini and full UI modes
matching WinAMP's interface functionality
```

### Bug Fix
```
fix(player): resolve GStreamer spurious EOS signals

Fix issue where GStreamer sends spurious EOS signals
when resuming playback, causing tracks to stop prematurely
```

### Refactor
```
refactor(playlist): improve track index handling

Refactor playlist index logic to properly handle
shuffled and ordered playlist navigation
```

### Documentation
```
docs: update BUILDING.md with termux dependencies
```

## Commit Count Estimation for TODO Items

Based on complexity and scope, here are estimated commit counts for each TODO item:

### High Priority Features
- Add equalizer functionality with preset and custom bands: 3-5 commits
- Implement crossfading between tracks: 2-3 commits
- Add audio format conversion/export feature: 4-6 commits
- Add album art display in the interface: 2-3 commits

### UI/UX Improvements
- Add support for custom WinAMP skins: 5-7 commits
- Implement keyboard shortcuts for all major functions: 2-4 commits
- Add high DPI scaling support: 2-3 commits
- Improve playlist management with drag-and-drop reordering: 3-5 commits
- Add search functionality within playlist: 2-3 commits

### Audio Enhancement
- Add gapless playback support: 3-4 commits
- Implement audio normalization/LUFS control: 3-5 commits
- Add support for ReplayGain metadata: 2-3 commits
- Improve spectrum analyzer visualization quality: 3-4 commits

### Platform Support
- Add Windows build support: 4-6 commits
- Add macOS build support: 4-6 commits
- Add Wayland support alongside X11: 3-5 commits
- Optimize for different screen sizes and orientations: 2-4 commits

### Code Quality & Maintenance
- Add unit tests for core functionality: 4-8 commits
- Implement proper logging system: 2-3 commits
- Add memory leak detection and prevention: 2-4 commits
- Add internationalization (i18n) support: 3-5 commits
- Refactor Player class to support multiple backends: 5-8 commits

### Bug Fixes & Performance
- Fix potential race conditions in UI updates: 1-2 commits
- Improve memory usage with large playlists: 2-3 commits
- Add error handling for file I/O operations: 2-3 commits
- Optimize visualizer performance (currently uses fake visualization): 3-5 commits
- Fix potential crashes when removing playlist items during playback: 1-2 commits

### Additional Features
- Add YouTube integration using yt-dlp CLI for audio extraction: 4-6 commits
- Implement sleep timer functionality: 2-3 commits
- Add file metadata editing capabilities: 3-5 commits
- Create notification system for track changes: 2-4 commits
- Add support for streaming audio services: 3-5 commits
- Implement bookmark functionality for long audio files: 2-3 commits
- Add podcast playback features: 3-5 commits

**Total Estimated Commits: 67-105 commits**

## Technical Considerations for Implementation

### Audio Processing Challenges
- Equalizer implementation will require real-time audio filtering with minimal latency
- Crossfading needs to handle different sample rates and audio formats
- Gapless playback requires careful buffer management in GStreamer pipeline

### Platform-Specific Issues
- Windows/macOS builds will need different GStreamer plugin handling
- Custom WinAMP skins may require Cairo surface manipulation for drawing
- High DPI scaling in GTK requires careful widget size calculations

### Performance Considerations
- Large playlist memory usage can be optimized with lazy loading
- Visualizer performance should be GPU-accelerated where possible
- YouTube integration via yt-dlp may introduce network dependency issues

### Architecture Recommendations
- Multi-backend player architecture should allow runtime switching between GStreamer, SDL, etc.
- Internationalization should use gettext-compatible string marking
- File I/O error handling should include permission checks and retry logic

## Testing Strategies for Implementation

### Unit Testing Approach
- Player class should have mock GStreamer backend for testing
- Playlist manager needs tests for shuffle/repeat logic
- UI components require headless testing with GtkTest

### Integration Testing
- Audio playback flow tests with sample files
- Playlist operations tests (add, remove, reorder)
- Cross-platform compatibility tests

### Performance Testing
- Memory usage monitoring with large playlists
- CPU usage optimization for visualizer
- Audio latency measurements for real-time features

### Platform-Specific Testing
- X11/Wayland display rendering tests
- Audio output testing across different backends
- File system access in Termux environment

## Project-Specific Coding Standards

### GTK/GStreamer Integration
- Always use `g_object_ref`/`g_object_unref` for GTK object lifecycle management
- GStreamer elements should be properly set to NULL state before unref
- Use `g_idle_add` for UI updates from GStreamer callbacks to avoid threading issues

### Memory Management
- RAII pattern for GStreamer resource management
- Smart pointer usage where appropriate for automatic cleanup
- Avoid memory leaks when switching between mini/full UI modes

### Code Organization
- Keep UI logic separate from audio playback logic
- Use consistent naming conventions (e.g., `on[Event]` for callbacks)
- Maintain WinAMP-like UI behavior consistency

## Common Pitfalls and Anti-Patterns

### Threading Issues
- Avoid calling GTK functions from GStreamer threads directly
- Don't update UI state without proper synchronization
- Be careful with lambda captures in GStreamer callbacks

### Resource Management
- Forgetting to unref GStreamer elements can cause memory leaks
- Not properly handling file paths in Termux environment
- GTK widget reference counting mistakes

### Audio Playback Gotchas
- GStreamer state transitions need proper error checking
- Position/duration queries should handle NULL returns
- Seek operations should respect current playback state