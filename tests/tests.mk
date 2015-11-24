# Unit tests
ifeq ($(HAS_cmocka), yes)
include tests/unit.mk
else
$(warning cmocka not found, skipping unit tests)
endif

# Integration tests with Deckard
deckard_DIR := tests/deckard
TESTS := sets/resolver
TEMPLATE := template/kresd.j2
$(deckard_DIR)/Makefile:
	@git submodule update --init
check-integration: $(deckard_DIR)/Makefile
	@cd $(deckard_DIR); git submodule update --init
	@make -C $(deckard_DIR) TESTS=$(TESTS) DAEMON=$(abspath daemon/kresd) TEMPLATE=$(TEMPLATE)
deckard: check-integration

# Targets
tests: check-unit
tests-clean: $(foreach test,$(tests_BIN),$(test)-clean)

.PHONY: tests tests-clean check-integration deckard
