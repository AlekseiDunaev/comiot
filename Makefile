# Copyright (C) 2006-2023 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=comiot
PKG_VERSION:=1.0.1
PKG_RELEASE:=5
PKG_MAINTAINER:=Aleksei Dunaev 
PKG_LICENSE:=GPL-2
# PKG_CONFIG_DEPENDS:=libsqlite3 libconfig libmosquitto
PKG_CONFIG_DEPENDS:=libsqlite3 libmosquitto libuci

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

TARGET_LDFLAGS+= \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib \
	# Add \ to end next 2 lines if uncomment it.
	# -Wl,-rpath-link=$(STAGING_DIR)/usr/lib/libconfig/lib
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib/sqlite/lib /
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib/mosquitto/lib /
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib/libuci/lib

define Package/comiot
	SECTION:=utils
	CATEGORY:=Utilities
	# DEPENDS:=+libsqlite3 +libconfig +libmosquitto
	DEPENDS:=+libsqlite3 +libmosquitto +libuci
	TITLE:=Read IoT data from COM port and public parse data in remote Influx db and local sqlite3 db.
	URL:=https://github.com/AlekseiDunaev/comiot
	MENU:=1
endef

define Package/comiot/description
	Read IoT data from COM port and public parse data in remote Influx db. Example SQLite is a sample program built using libsqlite3
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Build/Configure
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) $(TARGET_CONFIGURE_OPTS)
endef

define Package/comiot/install

	$(CP) ./files/* $(1)/
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/comiot $(1)/usr/bin/

	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/comiot $(1)/etc/config/comiot
endef

$(eval $(call BuildPackage,comiot))
