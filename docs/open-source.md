# Open Source Project Guide

HydroDock is maintained as an open-source project with two goals: build a useful hydration monitor and make the engineering process approachable for contributors.

## Project tone

The project should be practical, respectful, and beginner-friendly. Documentation should explain why a decision was made, not only what changed.

## Issue labels to consider

When repository labels are configured, these labels are recommended:

- `good first issue` for small, well-scoped tasks;
- `documentation` for docs-only improvements;
- `hardware` for wiring, enclosure, load-cell, and validation work;
- `firmware` for ESP-IDF and C++ changes;
- `architecture` for design discussions that affect component boundaries.

## What belongs in the firmware

The firmware should own hardware behavior, event creation, local reminders, local storage, and sync mechanics. It should not own long-term analytics, cloud dashboards, nutrition data, or health-platform aggregation.

## What belongs in external applications

External applications should consume event history and provide richer experiences such as charts, cloud synchronization, health integrations, correlations, and wearable aggregation.
