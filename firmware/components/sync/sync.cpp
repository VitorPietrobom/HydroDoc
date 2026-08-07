#include "sync/sync.hpp"
namespace hydromonitor::sync {
void SyncService::acknowledge(SyncCursor cursor) { cursor_ = cursor; }
SyncCursor SyncService::cursor() const { return cursor_; }
} // namespace hydromonitor::sync
