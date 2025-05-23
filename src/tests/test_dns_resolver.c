/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "platform.h"
#if PL_CONFIG_USE_UNIT_TESTS
#include "test_dns_resolver.h"
#include "McuUtility.h"
#include "dns_resolver.h"
#include "unity/unity.h"
#include "fff.h"

DEFINE_FFF_GLOBALS;
FAKE_VOID_FUNC(DnsResolver_Init);

void TestDnsResolver_Test(void) {
  DnsResolver_info_t info;
  int res;
  unsigned char buf[16];

  // DnsResolver_ResolveName(NULL, NULL, 0); // will fail and crash!
  res = DnsResolver_ResolveName("127.0.0.1", &info, 0);
  TEST_ASSERT_MESSAGE(res==0, "fixed IP does not need resolving");
  ip4addr_ntoa_r(&info.resolved_addr, buf, sizeof(buf));
  TEST_ASSERT(McuUtility_strcmp(buf, "127.0.0.1")==0);

  TEST_ASSERT_MESSAGE(DnsResolver_ResolveName("server", &info, -1)==-2, "negative timeout shall return -2");

  //DnsResolver_Init();
  //TEST_ASSERT_MESSAGE(DnsResolver_Init_fake.call_count==1, "call count shall be 1");
}

#endif /* PL_CONFIG_USE_UNIT_TESTS */