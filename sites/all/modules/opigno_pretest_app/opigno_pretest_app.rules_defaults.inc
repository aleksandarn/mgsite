<?php

/**
 * @file
 * Define default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function opigno_pretest_app_default_rules_configuration() {
  $items = array();
  $items['rules_get_courses_that_have_current_quiz_as_pre_test'] = entity_import('rules_config', '{ "rules_get_courses_that_have_current_quiz_as_pre_test" : {
      "LABEL" : "Get courses that have current quiz as pre-test",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "og", "opigno", "opigno_pretest_app", "quiz" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : {
        "quiz" : { "label" : "Quiz", "type" : "node" },
        "courses" : {
          "label" : "Courses",
          "type" : "list\u003Cnode\u003E",
          "parameter" : false
        }
      },
      "ACTION SET" : [
        { "entity_query" : {
            "USING" : {
              "type" : "node",
              "property" : "course_pretest_ref",
              "value" : [ "quiz" ]
            },
            "PROVIDE" : { "entity_fetched" : { "entities_fetched" : "Fetched entities" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "entities-fetched" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "list_add" : { "list" : [ "courses" ], "item" : [ "list-item" ], "unique" : 1 } }
            ]
          }
        }
      ],
      "PROVIDES VARIABLES" : [ "courses" ]
    }
  }');
  $items['rules_quiz_is_a_pre_test'] = entity_import('rules_config', '{ "rules_quiz_is_a_pre_test" : {
      "LABEL" : "Quiz is a pre-test",
      "PLUGIN" : "rule condition set",
      "TAGS" : [ "og", "opigno", "quiz", "opigno_pretest_app" ],
      "REQUIRES" : [ "rules_conditional", "rules" ],
      "USES VARIABLES" : { "quiz" : { "label" : "Quiz", "type" : "node" } },
      "DO" : [
        { "component_rules_get_courses_that_have_current_quiz_as_pre_test" : {
            "USING" : { "quiz" : [ "quiz" ] },
            "PROVIDE" : { "courses" : { "courses" : "Courses" } }
          }
        }
      ],
      "RESULT" : [ { "NOT data_is_empty" : { "data" : [ "courses" ] } } ]
    }
  }');
  $items['rules_activate_membership_when_required_pretest_is_passed'] = entity_import('rules_config', '{ "rules_activate_membership_when_required_pretest_is_passed" : {
      "LABEL" : "Activate membership when required pre-test is passed",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "og", "opigno", "quiz", "opigno_pretest_app" ],
      "REQUIRES" : [ "rules", "rules_conditional", "rules_i18n", "og", "opigno_quiz_app" ],
      "ON" : [ "opigno_quiz_app_rules_quiz_passed" ],
      "IF" : [ { "component_rules_quiz_is_a_pre_test" : { "quiz" : [ "node" ] } } ],
      "DO" : [
        { "component_rules_get_courses_that_have_current_quiz_as_pre_test" : {
            "USING" : { "quiz" : [ "node" ] },
            "PROVIDE" : { "courses" : { "courses" : "Courses" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "courses" ] },
            "ITEM" : { "course" : "Course" },
            "DO" : [
              { "CONDITIONAL" : [
                  {
                    "IF" : { "og_user_in_group" : {
                        "account" : [ "user" ],
                        "group" : [ "course" ],
                        "states" : { "value" : { "2" : "2" } }
                      }
                    },
                    "DO" : [
                      { "component_rules_activate_group_membership" : { "course" : [ "course" ], "user" : [ "user" ] } },
                      { "drupal_message" : { "message" : "Your membership for \u003Ca href=\u0022[site:url]node\/[course:nid]\u0022\u003E[course:title]\u003C\/a\u003E has been approved." } }
                    ]
                  }
                ]
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_show_message_if_course_has_a_pre_test'] = entity_import('rules_config', '{ "rules_show_message_if_course_has_a_pre_test" : {
      "LABEL" : "Show message if course has a pre-test",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "og", "opigno", "quiz", "opigno_pretest_app" ],
      "REQUIRES" : [ "rules", "og" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "course" : "course" } } } },
        { "og_user_in_group" : {
            "account" : [ "site:current-user" ],
            "group" : [ "node" ],
            "states" : { "value" : { "2" : "2" } }
          }
        },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "course_pretest_ref" } },
        { "NOT data_is_empty" : { "data" : [ "node:course-pretest-ref" ] } }
      ],
      "DO" : [
        { "entity_fetch" : {
            "USING" : { "type" : "node", "id" : [ "node:course-pretest-ref:nid" ] },
            "PROVIDE" : { "entity_fetched" : { "pre_test" : "Pre test" } }
          }
        },
        { "drupal_message" : {
            "message" : "You may not access this course\u0027s content before you successfully passed \u003Ca href=\u0022[site:url]node\/[pre-test:nid]\u0022\u003Ethe following quiz\u003C\/a\u003E.",
            "type" : "warning"
          }
        }
      ]
    }
  }');
  return $items;
}
