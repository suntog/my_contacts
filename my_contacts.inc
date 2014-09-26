<?php
/**
 * @file
 * Non-hook functions.
 */


/**
 * Show old portal info as My Page.
 */

function my_contacts_contact_list() {
  drupal_add_css(drupal_get_path('module', 'my_contacts') . '/css/my_contacts.css');
  drupal_add_js(drupal_get_path('module', 'my_contacts') . '/js/my_contacts.js');
  $arg0 = arg(0);
  $arg1 = arg(1);
  if (arg(0) == 'user') {
    $uid = arg(1);
  } else {
    global $user;
    $uid = $user->uid;
  }

  $content =  '<div id="my-contacts-block-expand-wrapper">';
  $content .=   '<button id="my-contacts-block-expand">expand</button>';
  $content .= '</div><!-- my-contacts-block-expand-wrapper -->';
  $content .= '<div id="my-contacts-list-wrapper">';
//  $result = db_query('SELECT cid FROM {babson_profile_contacts} WHERE uid = :uid', array('uid' => $uid));
  $sql = db_select('babson_profile_contacts', 'bpc');
  $sql->fields('bpc', array('cid'));
  $sql->condition('bpc.uid', $uid);
  $result = $sql->execute();

  foreach($result as $row){

    $account = user_load($row->cid);
    $email = isset($account->mail) ? $account->mail : '';
    $firstname = isset($account->field_first_name['und'][0]['value']) ? $account->field_first_name['und'][0]['value'] : '';
    $lastname = isset($account->field_last_name['und'][0]['value']) ? $account->field_last_name['und'][0]['value'] : '';
    $fullname = $firstname . ' ' . $lastname;
    $content .= '<div class="my-contacts no-text">';
    $content .=   '<div class="username-wrapper">';
    $content .=     theme('username', array('account' => $account));
    $content .=   '</div><!-- .username-wrapper -->';
    $content .=   '<div class="username-button-wrapper">';


    if (module_exists('droogle_drive')) {
      $content .= theme('droogle_drive_create_invite_button', array('email' => $email, 'fullname' => $fullname, 'text' => FALSE));
    }
    if (module_exists('droogle_hangout')) {
      $content .= theme('droogle_hangout_invite_button', array('email' => $email, 'fullname' => $fullname, 'text' => FALSE));
      $content .= theme('droogle_hangout_email_button', array('email' => $email, 'fullname' => $fullname, 'text' => FALSE));
    }
    if (module_exists('jabber') && module_exists('droogle_hangout')) {
      $content .= theme('droogle_hangout_chat_button', array('email' => $email, 'fullname' => $fullname, 'text' => FALSE));
    }

    $content .=   '</div><!-- .username-button-wrapper -->';
    $content .= '</div><!-- .my-contacts -->';
  }
  $content .= '</div><!-- .my-contacts-list-wrapper -->';
  return $content;
}

/**
 *  Implements hook_theme() for Create Contact, for a fast contact list.
 *  Phineas, isn't this function overused in droogle docs?
 *  Why yes Mr. Drupal Developer, yes it is.
 */

function theme_my_contacts_create_contact($variables) {

  $uid = $variables['uid'];
  $cid = $variables['cid'];
  if ($variables['text'] == TRUE) {
    $text = 'Add Contact';
  } else {
    $text = '';
  }

  $content  = '<div class="my-contacts-create-contact-btn-wrapper">';
  $content .=   '<button class="my-contacts-create-contact-btn" name="create-contact">' . $text;
  $content .=     '<span class="button-cid" style="display:none;">' . $cid . '</span>';
  $content .=     '<span class="button-uid" style="display:none;">' . $uid . '</span>';
  $content .=     '<span class="text-balloon-wrapper"><span class="text-balloon">';
  $content .=       'Add Contact Info to Profile Page';
  $content .=     '</span></span>';
  $content .=	'</button>';
  $content .= '</div><!-- .my-contacts-create-contact-btn-wrapper -->';
  return $content;
}

function my_contacts_ajax_add_contact() {
  $success = 'Got there and back';
  $cid = $_POST['cid'];
  $uid = $_POST['uid'];

  if ($uid == NULL) {
    global $user;
    $uid = $user->uid;
  }
  if ($uid == $cid) {
    $success = "HEY! You can't add a contact of yourself!";
  } else {
    $has_cid = db_query("SELECT cid FROM {babson_profile_contacts} bpc WHERE bpc.cid = :cid and bpc.uid = :uid", array(':cid' => $cid, ':uid' => $uid))->fetchfield();
    if ($has_cid != FALSE) {
      $success = "You already have this contact";
    } else {
      $contact_insert = db_insert('babson_profile_contacts')
        ->fields(array(
          'uid' => $uid,
          'cid' => $cid,
          'timestamp' => REQUEST_TIME,
        ))
        ->execute();

      $success = "Contact Added";
    }
  }

  $account = user_load($cid);
  drupal_json_output(
    array(
      'success' => $success,
    )
  );
}