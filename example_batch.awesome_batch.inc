<?php

use Drupal\Component\Uuid\Php;

/**
 * Batch execution.
 *
 * @param $number
 *   Number of times to execute.
 * @param array $context
 *   An array of contextual key/value information for rebuild batch process.
 */
function _awesome_batch($number, &$context) {
  if ($number && is_numeric($number)) {
    $message = \Drupal::translation()->formatPlural(
      $number,
      'One random string is listed below:', '@count random strings are listed below:'
    );
    \Drupal::messenger()->addMessage($message);

    // Initiate multistep processing.
    if (empty($context['sandbox'])) {
      $context['sandbox']['progress'] = 0;
      $context['sandbox']['max'] = $number;
    }

    // Process the next 100 if there are at least 100 left. Otherwise,
    // we process the remaining number.
    $batch_size = 100;
    $max = $context['sandbox']['progress'] + $batch_size;
    if ($max > $context['sandbox']['max']) {
      $max = $context['sandbox']['max'];
    }

    // Start where we left off last time.
    $start = $context['sandbox']['progress'];
    for ($i = $start; $i < $max; $i++) {
      $code = randomString();

      // We want to display the counter 1-based, not 0-based.
      $counter = $i + 1;
      \Drupal::messenger()->addMessage(t('Counter @counter and str @str', ['@counter' => $counter, '@str' => $code]));
      
      // To see the progress of individual batch.
      $context['message'] = t('Processing @i of @number', ['@i' => $i, '@number' => $number]);

      // Update our progress!
      $context['sandbox']['progress']++;
    }

    // Multistep processing : report progress.
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }
}

/**
 * @return string
 */
function randomString() {
  $uuid = new Php();
  $code = $uuid->generate();
  $code = strtoupper($code);
  return $code;
}
