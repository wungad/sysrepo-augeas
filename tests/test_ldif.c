/**
 * @file test_ldif.c
 * @author Michal Vasko <mvasko@cesnet.cz>
 * @brief ldif SR DS plugin test
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
#define AUG_TEST_INPUT_FILES AUG_CONFIG_FILES_DIR "/ldif;" AUG_CONFIG_FILES_DIR "/ldif2"
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

#define AUG_TEST_MODULE "ldif"

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
            "  <changes>\n"
            "    <version>1</version>\n"
            "    <record-list>\n"
            "      <_seq>1</_seq>\n"
            "      <value>cn=foo,dc=example,dc=com</value>\n"
            "      <change-type>\n"
            "        <change-type>changetype</change-type>\n"
            "        <value>delete</value>\n"
            "      </change-type>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>2</_seq>\n"
            "      <value>cn=simple,dc=example,dc=com</value>\n"
            "      <change-control-list>\n"
            "        <_id>1</_id>\n"
            "        <control>\n"
            "          <ldapoid>1.2.3.4</ldapoid>\n"
            "        </control>\n"
            "      </change-control-list>\n"
            "      <change-control-list>\n"
            "        <_id>2</_id>\n"
            "        <control>\n"
            "          <ldapoid>1.2.3.4</ldapoid>\n"
            "          <criticality>true</criticality>\n"
            "        </control>\n"
            "      </change-control-list>\n"
            "      <change-control-list>\n"
            "        <_id>3</_id>\n"
            "        <control>\n"
            "          <ldapoid>1.2.3.4</ldapoid>\n"
            "          <criticality>true</criticality>\n"
            "          <value>\n"
            "            <value>foo bar</value>\n"
            "          </value>\n"
            "        </control>\n"
            "      </change-control-list>\n"
            "      <change-control-list>\n"
            "        <_id>4</_id>\n"
            "        <control>\n"
            "          <ldapoid>1.2.3.4</ldapoid>\n"
            "          <criticality>true</criticality>\n"
            "          <value>\n"
            "            <base64>Zm9vIGJhcg==</base64>\n"
            "          </value>\n"
            "        </control>\n"
            "      </change-control-list>\n"
            "      <change-type>\n"
            "        <change-type>changetype</change-type>\n"
            "        <value>add</value>\n"
            "      </change-type>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>1</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>cn</attr-type>\n"
            "          <value>simple</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>3</_seq>\n"
            "      <value>cn=foo bar,dc=example,dc=com</value>\n"
            "      <change-type>\n"
            "        <change-type>changeType</change-type>\n"
            "        <value>modify</value>\n"
            "      </change-type>\n"
            "      <change-modify>\n"
            "        <_id>1</_id>\n"
            "        <add>\n"
            "          <attr-type>telephoneNumber</attr-type>\n"
            "          <attr-val-spec-list>\n"
            "            <_id>1</_id>\n"
            "            <attr-val-spec>\n"
            "              <attr-type>telephoneNumber</attr-type>\n"
            "              <value>+1 123 456 789</value>\n"
            "            </attr-val-spec>\n"
            "          </attr-val-spec-list>\n"
            "        </add>\n"
            "      </change-modify>\n"
            "      <change-modify>\n"
            "        <_id>2</_id>\n"
            "        <replace>\n"
            "          <attr-type>homePostalAddress</attr-type>\n"
            "          <option>lang-fr</option>\n"
            "          <attr-val-spec-list>\n"
            "            <_id>1</_id>\n"
            "            <attr-val-spec>\n"
            "              <attr-type>homePostalAddress</attr-type>\n"
            "              <option>lang-fr</option>\n"
            "              <value>34 rue de Seine</value>\n"
            "            </attr-val-spec>\n"
            "          </attr-val-spec-list>\n"
            "        </replace>\n"
            "      </change-modify>\n"
            "      <change-modify>\n"
            "        <_id>3</_id>\n"
            "        <delete>\n"
            "          <attr-type>telephoneNumber</attr-type>\n"
            "        </delete>\n"
            "      </change-modify>\n"
            "      <change-modify>\n"
            "        <_id>4</_id>\n"
            "        <replace>\n"
            "          <attr-type>telephoneNumber</attr-type>\n"
            "          <attr-val-spec-list>\n"
            "            <_id>1</_id>\n"
            "            <attr-val-spec>\n"
            "              <attr-type>telephoneNumber</attr-type>\n"
            "              <base64>KzEgMTIzIDQ1NiA3ODk=</base64>\n"
            "            </attr-val-spec>\n"
            "          </attr-val-spec-list>\n"
            "        </replace>\n"
            "      </change-modify>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>4</_seq>\n"
            "      <value>cn=foo,dc=example,dc=com</value>\n"
            "      <change-type>\n"
            "        <change-type>changetype</change-type>\n"
            "        <value>moddn</value>\n"
            "      </change-type>\n"
            "      <change-modrdn>\n"
            "        <_id>1</_id>\n"
            "        <newrdn>\n"
            "          <value>cn=bar</value>\n"
            "        </newrdn>\n"
            "      </change-modrdn>\n"
            "      <change-modrdn>\n"
            "        <_id>2</_id>\n"
            "        <deleteoldrdn>0</deleteoldrdn>\n"
            "      </change-modrdn>\n"
            "      <change-modrdn>\n"
            "        <_id>3</_id>\n"
            "        <newsuperior>\n"
            "          <value>dc=example,dc=net</value>\n"
            "        </newsuperior>\n"
            "      </change-modrdn>\n"
            "    </record-list>\n"
            "  </changes>\n"
            "</ldif>\n"
            "<ldif xmlns=\"aug:ldif\">\n"
            "  <config-file>" AUG_CONFIG_FILES_DIR "/ldif2</config-file>\n"
            "  <content>\n"
            "    <version>1</version>\n"
            "    <record-list>\n"
            "      <_seq>1</_seq>\n"
            "      <value>cn=foo bar,dc=example,dc=com</value>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>1</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>ou</attr-type>\n"
            "          <value>example value</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>2</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>cn</attr-type>\n"
            "          <base64>Zm9vIGJhcg==</base64>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>3</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>telephoneNumber</attr-type>\n"
            "          <option>foo</option>\n"
            "          <option>bar</option>\n"
            "          <value>+1 123 456 789</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>4</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>binary</attr-type>\n"
            "          <option>foo</option>\n"
            "          <url>file:///file/something</url>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>2</_seq>\n"
            "      <value>cn=simple,dc=example,dc=com</value>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>1</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>cn</attr-type>\n"
            "          <value>simple</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>2</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>test</attr-type>\n"
            "          <value>split line starts with\n"
            "  :colon</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>3</_seq>\n"
            "      <base64>Y249c2ltcGxlLGRjPWV4YW1wbGUsZGM9Y29t</base64>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>1</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>cn</attr-type>\n"
            "          <value>simple</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "    </record-list>\n"
            "    <record-list>\n"
            "      <_seq>4</_seq>\n"
            "      <value>cn=simple,dc=exam\n"
            "    ple,dc=com</value>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>1</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>cn</attr-type>\n"
            "          <value>simple</value>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "      <attr-val-spec-list>\n"
            "        <_id>2</_id>\n"
            "        <attr-val-spec>\n"
            "          <attr-type>telephoneNumber</attr-type>\n"
            "          <base64>KzEgMTIzIDQ1\n"
            "  NiA3ODk=</base64>\n"
            "        </attr-val-spec>\n"
            "      </attr-val-spec-list>\n"
            "    </record-list>\n"
            "  </content>\n"
            "</" AUG_TEST_MODULE ">\n");
    free(str);
}

int
main(void)
{
    /* BUG augeas is not able to store any changes in these config files */
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_teardown(test_load, tteardown),
    };

    return cmocka_run_group_tests(tests, setup_f, tteardown_glob);
}
