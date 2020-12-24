# mtfind
Simple multi-threaded grep-like utility.

Patterns may contain '?' wildcard, matching any character.

Limitations:
  * Only ascii charset is supported. 
  * Matches cannot cross line bondary.
  * Matches cannot overlap.
