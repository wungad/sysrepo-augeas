/**
 * @file test_ceph.c
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief ceph SR DS plugin test
 *
 * @copyright
 * Copyright (c) 2022 Deutsche Telekom AG.
 * Copyright (c) 2022 CESNET, z.s.p.o.
 *
 * This source code is licensed under BSD 3-Clause License (the "License").
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://opensource.org/licenses/BSD-3-Clause
 */

#include "tconfig.h"

/* augeas SR DS plugin */
#define AUG_TEST_INPUT_FILES AUG_CONFIG_FILES_DIR "/ceph"
#include "srds_augeas.c"
#include "srdsa_init.c"
#include "srdsa_load.c"
#include "srdsa_store.c"
#include "srdsa_common.c"

#include <assert.h>
#include <dlfcn.h>
#include <setjmp.h>
#include <stdarg.h>
#include <unistd.h>

#include <cmocka.h>
#include <libyang/libyang.h>
#include <sysrepo/plugins_datastore.h>

#define AUG_TEST_MODULE "ceph"

static int
setup_f(void **state)
{
    return tsetup_glob(state, AUG_TEST_MODULE, &srpds__, AUG_TEST_INPUT_FILES);
}

static void
test_load(void **state)
{
    struct tstate *st = (struct tstate *)*state;
    char *str;

    assert_int_equal(SR_ERR_OK, st->ds_plg->load_cb(st->mod, SR_DS_STARTUP, NULL, 0, &st->data));
    lyd_print_mem(&str, st->data, LYD_XML, LYD_PRINT_WITHSIBLINGS);

    assert_string_equal(str,
            "<" AUG_TEST_MODULE " xmlns=\"aug:" AUG_TEST_MODULE "\">\n"
            "  <config-file>" AUG_CONFIG_FILES_DIR "/" AUG_TEST_MODULE "</config-file>\n"
            "  <record-list>\n"
            "    <_id>1</_id>\n"
            "    <record>\n"
            "      <label>global</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>fsid</entry-re>\n"
            "          <value>{UUID}</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>public network</entry-re>\n"
            "          <value>192.168.0.0/24</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>cluster network</entry-re>\n"
            "          <value>192.168.0.0/24</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>pid file</entry-re>\n"
            "          <value>/var/run/ceph/$name.pid</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>max open files</entry-re>\n"
            "          <value>131072</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>auth cluster required</entry-re>\n"
            "          <value>cephx</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>7</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>auth service required</entry-re>\n"
            "          <value>cephx</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>8</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>auth client required</entry-re>\n"
            "          <value>cephx</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>9</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>cephx require signatures</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>10</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>cephx cluster require signatures</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>11</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>cephx service require signatures</entry-re>\n"
            "          <value>false</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>12</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>keyring</entry-re>\n"
            "          <value>/etc/ceph/$cluster.$name.keyring</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>13</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd pool default size</entry-re>\n"
            "          <value>3</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>14</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd pool default min size</entry-re>\n"
            "          <value>2</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>15</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd pool default pg num</entry-re>\n"
            "          <value>128</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>16</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd pool default pgp num</entry-re>\n"
            "          <value>128</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>17</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd pool default crush rule</entry-re>\n"
            "          <value>0</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>18</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd crush chooseleaf type</entry-re>\n"
            "          <value>1</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>19</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>log file</entry-re>\n"
            "          <value>/var/log/ceph/$cluster-$name.log</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>20</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>log to syslog</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>21</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>ms bind ipv6</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>2</_id>\n"
            "    <record>\n"
            "      <label>mon</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon initial members</entry-re>\n"
            "          <value>mycephhost</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon host</entry-re>\n"
            "          <value>cephhost01,cephhost02</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon addr</entry-re>\n"
            "          <value>192.168.0.101,192.168.0.102</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon data</entry-re>\n"
            "          <value>/var/lib/ceph/mon/$name</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon clock drift allowed</entry-re>\n"
            "          <value>.15</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon clock drift warn backoff</entry-re>\n"
            "          <value>30</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>7</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon osd full ratio</entry-re>\n"
            "          <value>.95</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>8</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon osd nearfull ratio</entry-re>\n"
            "          <value>.85</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>9</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon osd down out interval</entry-re>\n"
            "          <value>600</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>10</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon osd report timeout</entry-re>\n"
            "          <value>300</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>11</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug ms</entry-re>\n"
            "          <value>1</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>12</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug mon</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>13</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug paxos</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>14</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug auth</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>3</_id>\n"
            "    <record>\n"
            "      <label>mon.alpha</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>alpha</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon addr</entry-re>\n"
            "          <value>192.168.0.10:6789</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>4</_id>\n"
            "    <record>\n"
            "      <label>mon.beta</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>beta</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon addr</entry-re>\n"
            "          <value>192.168.0.11:6789</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>5</_id>\n"
            "    <record>\n"
            "      <label>mon.gamma</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>gamma</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mon addr</entry-re>\n"
            "          <value>192.168.0.12:6789</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>6</_id>\n"
            "    <record>\n"
            "      <label>mds</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>keyring</entry-re>\n"
            "          <value>/var/lib/ceph/mds/$name/keyring</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mds standby replay</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug ms</entry-re>\n"
            "          <value>1</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug mds</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug journaler</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>mds cache memory limit</entry-re>\n"
            "          <value>2G</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>7</_id>\n"
            "    <record>\n"
            "      <label>mds.alpha</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>alpha</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>8</_id>\n"
            "    <record>\n"
            "      <label>mds.beta</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>beta</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>9</_id>\n"
            "    <record>\n"
            "      <label>osd</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd data</entry-re>\n"
            "          <value>/var/lib/ceph/osd/$name</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd recovery max active</entry-re>\n"
            "          <value>3</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd max backfills</entry-re>\n"
            "          <value>5</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd max scrubs</entry-re>\n"
            "          <value>2</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd mkfs type</entry-re>\n"
            "          <value>btrfs</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd mount options btrfs</entry-re>\n"
            "          <value>noatime,nodiratime</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>7</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd journal</entry-re>\n"
            "          <value>/var/lib/ceph/osd/$name/journal</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>8</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd check for log corruption</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>9</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd journal size</entry-re>\n"
            "          <value>2048</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>10</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>journal dio</entry-re>\n"
            "          <value>false</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>11</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug ms</entry-re>\n"
            "          <value>1</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>12</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug osd</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>13</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug filestore</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>14</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>debug journal</entry-re>\n"
            "          <value>20</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>15</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore max sync interval</entry-re>\n"
            "          <value>5</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>16</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore btrfs snap</entry-re>\n"
            "          <value>false</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>17</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore flusher</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>18</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore queue max ops</entry-re>\n"
            "          <value>500</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>19</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore merge threshold</entry-re>\n"
            "          <value>-10</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>20</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore split multiple</entry-re>\n"
            "          <value>2</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>21</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>filestore op threads</entry-re>\n"
            "          <value>4</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>22</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>osd crush update on start</entry-re>\n"
            "          <value>false</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>10</_id>\n"
            "    <record>\n"
            "      <label>osd.0</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>delta</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>11</_id>\n"
            "    <record>\n"
            "      <label>osd.1</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>epsilon</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>12</_id>\n"
            "    <record>\n"
            "      <label>osd.2</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>zeta</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>13</_id>\n"
            "    <record>\n"
            "      <label>osd.3</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>eta</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>14</_id>\n"
            "    <record>\n"
            "      <label>client</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache size</entry-re>\n"
            "          <value>33554432</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache max dirty</entry-re>\n"
            "          <value>25165824</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache target dirty</entry-re>\n"
            "          <value>16777216</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache max dirty age</entry-re>\n"
            "          <value>1.0</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rbd cache writethrough until flush</entry-re>\n"
            "          <value>true</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>7</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>admin socket</entry-re>\n"
            "          <value>/var/run/ceph/$cluster-$type.$id.$pid.$cctid.asok</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "  <record-list>\n"
            "    <_id>15</_id>\n"
            "    <record>\n"
            "      <label>client.radosgw.gateway</label>\n"
            "      <entry-re-list>\n"
            "        <_id>1</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rgw data</entry-re>\n"
            "          <value>/var/lib/ceph/radosgw/$name</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>2</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>host</entry-re>\n"
            "          <value>ceph-radosgw</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>3</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>keyring</entry-re>\n"
            "          <value>/etc/ceph/ceph.client.radosgw.keyring</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>4</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rgw socket path</entry-re>\n"
            "          <value>/var/run/ceph/ceph.radosgw.gateway.fastcgi.sock</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>5</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>log file</entry-re>\n"
            "          <value>/var/log/ceph/client.radosgw.gateway.log</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>6</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rgw print continue</entry-re>\n"
            "          <value>false</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "      <entry-re-list>\n"
            "        <_id>7</_id>\n"
            "        <entry-re>\n"
            "          <entry-re>rgw dns name</entry-re>\n"
            "          <value>radosgw.ceph.internal</value>\n"
            "        </entry-re>\n"
            "      </entry-re-list>\n"
            "    </record>\n"
            "  </record-list>\n"
            "</" AUG_TEST_MODULE ">\n");
    free(str);
}

static void
test_store_add(void **state)
{
    struct tstate *st = (struct tstate *)*state;
    struct lyd_node *entries, *node;

    /* load current data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->load_cb(st->mod, SR_DS_STARTUP, NULL, 0, &st->data));

    /* add some new list instances */
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='11']/record/entry-re-list[_id='2']/"
            "entry-re/entry-re", "filestore flusher", 0, &entries));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='11']/record/entry-re-list[_id='2']/"
            "entry-re/value", "false", 0, NULL));

    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='6']/record/entry-re-list[_id='7']/"
            "entry-re/entry-re", "host", 0, &entries));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='6']/record/entry-re-list[_id='7']/"
            "entry-re/value", "gama", 0, NULL));
    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='6']/record/entry-re-list[_id='1']", 0, &node));
    assert_int_equal(LY_SUCCESS, lyd_insert_before(node, entries));

    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='16']/record/label", "mds.omega", 0, &entries));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='16']/record/entry-re-list[_id='1']/"
            "entry-re/entry-re", "host", 0, NULL));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='16']/record/entry-re-list[_id='1']/"
            "entry-re/value", "omega", 0, NULL));
    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='8']", 0, &node));
    assert_int_equal(LY_SUCCESS, lyd_insert_after(node, entries));

    /* store new data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->store_cb(st->mod, SR_DS_STARTUP, st->data));

    /* diff */
    assert_int_equal(0, tdiff_files(state,
            "174a175\n"
            "> host=gama\n"
            "197a199,200\n"
            "> [mds.omega]\n"
            "> host=omega\n"
            "325a329\n"
            "> filestore flusher=false\n"));
}

static void
test_store_modify(void **state)
{
    struct tstate *st = (struct tstate *)*state;
    struct lyd_node *entries, *node;

    /* load current data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->load_cb(st->mod, SR_DS_STARTUP, NULL, 0, &st->data));

    /* modify some values */
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='15']/record/label",
            "client.radosgw.gateway.new", LYD_NEW_PATH_UPDATE, NULL));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='15']/record/entry-re-list[_id='2']/"
            "entry-re/value", "ceph-radosgw-update", LYD_NEW_PATH_UPDATE, NULL));
    assert_int_equal(LY_SUCCESS, lyd_new_path(st->data, NULL, "record-list[_id='15']/record/entry-re-list[_id='6']/"
            "entry-re/entry-re", "rgw print update continue", LYD_NEW_PATH_UPDATE, NULL));

    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='14']/record/entry-re-list[_id='2']", 0, &entries));
    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='14']/record/entry-re-list[_id='6']", 0, &node));
    assert_int_equal(LY_SUCCESS, lyd_insert_after(node, entries));

    /* store new data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->store_cb(st->mod, SR_DS_STARTUP, st->data));

    /* diff */
    assert_int_equal(0, tdiff_files(state,
            "339a340,343\n"
            ">     rbd cache max dirty                = 25165824\n"
            ">     rbd cache target dirty             = 16777216\n"
            ">     rbd cache max dirty age            = 1.0\n"
            ">     rbd cache writethrough until flush = true\n"
            "353d356\n"
            "<     rbd cache max dirty                = 25165824\n"
            "361d363\n"
            "<     rbd cache target dirty             = 16777216\n"
            "367d368\n"
            "<     rbd cache max dirty age            = 1.0\n"
            "376d376\n"
            "<     rbd cache writethrough until flush = true\n"
            "384c384\n"
            "< [client.radosgw.gateway]\n"
            "---\n"
            "> [client.radosgw.gateway.new]\n"
            "387c387\n"
            "< host=ceph-radosgw\n"
            "---\n"
            "> host=ceph-radosgw-update\n"
            "391c391\n"
            "< rgw print continue=false\n"
            "---\n"
            "> rgw print update continue=false\n"));
}

static void
test_store_remove(void **state)
{
    struct tstate *st = (struct tstate *)*state;
    struct lyd_node *node;

    /* load current data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->load_cb(st->mod, SR_DS_STARTUP, NULL, 0, &st->data));

    /* remove list values */
    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='7']", 0, &node));
    lyd_free_tree(node);

    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='14']/record/entry-re-list[_id='4']", 0, &node));
    lyd_free_tree(node);
    assert_int_equal(LY_SUCCESS, lyd_find_path(st->data, "record-list[_id='14']/record/entry-re-list[_id='5']", 0, &node));
    lyd_free_tree(node);

    /* store new data */
    assert_int_equal(SR_ERR_OK, st->ds_plg->store_cb(st->mod, SR_DS_STARTUP, st->data));

    /* diff */
    assert_int_equal(0, tdiff_files(state,
            "192,194d191\n"
            "< [mds.alpha]\n"
            "<     host                       = alpha\n"
            "< \n"
            "361d357\n"
            "<     rbd cache target dirty             = 16777216\n"
            "367d362\n"
            "<     rbd cache max dirty age            = 1.0\n"));
}

int
main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_teardown(test_load, tteardown),
        cmocka_unit_test_teardown(test_store_add, tteardown),
        cmocka_unit_test_teardown(test_store_modify, tteardown),
        cmocka_unit_test_teardown(test_store_remove, tteardown),
    };

    return cmocka_run_group_tests(tests, setup_f, tteardown_glob);
}
