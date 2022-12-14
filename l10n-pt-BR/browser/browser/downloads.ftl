# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The title and aria-label attributes are used by screen readers to describe
## the Downloads Panel.

downloads-window =
    .title = Downloads
downloads-panel =
    .aria-label = Downloads

##

# The style attribute has the width of the Downloads Panel expressed using
# a CSS unit. The longest labels that should fit are usually those of
# in-progress and blocked downloads.
downloads-panel-items =
    .style = width: 35em
downloads-cmd-pause =
    .label = Pausar
    .accesskey = P
downloads-cmd-resume =
    .label = Continuar
    .accesskey = R
downloads-cmd-cancel =
    .tooltiptext = Cancelar
downloads-cmd-cancel-panel =
    .aria-label = Cancelar
# This message is only displayed on Windows and Linux devices
downloads-cmd-show-menuitem =
    .label = Abrir pasta
    .accesskey = r
# This message is only displayed on macOS devices
downloads-cmd-show-menuitem-mac =
    .label = Mostrar no Finder
    .accesskey = F
downloads-cmd-show-menuitem-2 =
    .label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
    .accesskey = M
downloads-cmd-use-system-default =
    .label = Abrir no visor do sistema
    .accesskey = v
# We can use the same accesskey as downloads-cmd-always-open-similar-files.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-use-system-default =
    .label = Sempre abrir no visor do sistema
    .accesskey = m
downloads-cmd-show-button =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta
        }
downloads-cmd-show-panel =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta
        }
downloads-cmd-show-description =
    .value =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Abrir pasta
        }
# We can use the same accesskey as downloads-cmd-always-use-system-default.
# Both should not be visible in the downloads context menu at the same time.
downloads-cmd-always-open-similar-files =
    .label = Sempre abrir arquivos semelhantes
    .accesskey = S
downloads-cmd-show-button-2 =
    .tooltiptext =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-panel-2 =
    .aria-label =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-description-2 =
    .value =
        { PLATFORM() ->
            [macos] Mostrar no Finder
           *[other] Mostrar na pasta
        }
downloads-cmd-show-downloads =
    .label = Mostrar pasta de downloads
downloads-cmd-retry =
    .tooltiptext = Repetir
downloads-cmd-retry-panel =
    .aria-label = Repetir
downloads-cmd-go-to-download-page =
    .label = Ir para a p??gina de download
    .accesskey = d
downloads-cmd-copy-download-link =
    .label = Copiar link do download
    .accesskey = l
downloads-cmd-remove-from-history =
    .label = Remover do hist??rico
    .accesskey = E
downloads-cmd-clear-list =
    .label = Limpar o painel de visualiza????o
    .accesskey = a
downloads-cmd-clear-downloads =
    .label = Limpar downloads
    .accesskey = d
# This command is shown in the context menu when downloads are blocked.
downloads-cmd-unblock =
    .label = Permitir download
    .accesskey = o
# This is the tooltip of the action button shown when malware is blocked.
downloads-cmd-remove-file =
    .tooltiptext = Remover arquivo
downloads-cmd-remove-file-panel =
    .aria-label = Remover arquivo
# This is the tooltip of the action button shown when potentially unwanted
# downloads are blocked. This opens a dialog where the user can choose
# whether to unblock or remove the download. Removing is the default option.
downloads-cmd-choose-unblock =
    .tooltiptext = Remover arquivo ou permitir o download
downloads-cmd-choose-unblock-panel =
    .aria-label = Remover arquivo ou permitir o download
# This is the tooltip of the action button shown when uncommon downloads are
# blocked.This opens a dialog where the user can choose whether to open the
# file or remove the download. Opening is the default option.
downloads-cmd-choose-open =
    .tooltiptext = Abrir ou remover arquivo
downloads-cmd-choose-open-panel =
    .aria-label = Abrir ou remover arquivo
# Displayed when hovering a blocked download, indicates that it's possible to
# show more information for user to take the next action.
downloads-show-more-information =
    .value = Mostrar mais informa????es
# Displayed when hovering a complete download, indicates that it's possible to
# open the file using an app available in the system.
downloads-open-file =
    .value = Abrir arquivo

## Displayed when the user clicked on a download in process. Indicates that the
## downloading file will be opened after certain amount of time using an app
## available in the system.
## Variables:
##   $hours (number) - Amount of hours left till the file opens.
##   $seconds (number) - Amount of seconds left till the file opens.
##   $minutes (number) - Amount of minutes till the file opens.

downloading-file-opens-in-hours-and-minutes = Ser?? aberto em { $hours }h { $minutes }m???
downloading-file-opens-in-minutes = Ser?? aberto em { $minutes }m???
downloading-file-opens-in-minutes-and-seconds = Ser?? aberto em { $minutes }m { $seconds }s???
downloading-file-opens-in-seconds = Ser?? aberto em { $seconds }s???
downloading-file-opens-in-some-time = Ser?? aberto quando concluir???

##

# Displayed when hovering a download which is able to be retried by users,
# indicates that it's possible to download this file again.
downloads-retry-download =
    .value = Tentar baixar novamente
# Displayed when hovering a download which is able to be cancelled by users,
# indicates that it's possible to cancel and stop the download.
downloads-cancel-download =
    .value = Cancelar download
# This string is shown at the bottom of the Downloads Panel when all the
# downloads fit in the available space, or when there are no downloads in
# the panel at all.
downloads-history =
    .label = Mostrar todos os downloads
    .accesskey = s
# This string is shown at the top of the Download Details Panel, to indicate
# that we are showing the details of a single download.
downloads-details =
    .title = Detalhes do download

## Displayed when a site attempts to automatically download many files.
## Variables:
##   $num (number) - Number of blocked downloads.
##   $url (string) - The url of the suspicious site, stripped of http, https and www prefix.

downloads-files-not-downloaded =
    { $num ->
        [one] Arquivo n??o baixado.
       *[other] { $num } arquivos n??o baixados.
    }
downloads-blocked-from-url = Downloads bloqueados de { $url }.
downloads-blocked-download-detailed-info = { $url } tentou baixar automaticamente v??rios arquivos. O site pode estar ruim, ou est?? tentando armazenar arquivos de spam em seu dispositivo.

##

downloads-clear-downloads-button =
    .label = Limpar downloads
    .tooltiptext = Limpa downloads conclu??dos, cancelados e que falharam
# This string is shown when there are no items in the Downloads view, when it
# is displayed inside a browser tab.
downloads-list-empty =
    .value = Nenhum download.
# This string is shown when there are no items in the Downloads Panel.
downloads-panel-empty =
    .value = Nenhum download nessa sess??o.
# This is displayed in an item at the bottom of the Downloads Panel when there
# are more downloads than can fit in the list in the panel.
#   $count (number) - number of files being downloaded that are not shown in the
#                     panel list.
downloads-more-downloading =
    { $count ->
        [one] Mais { $count } arquivo sendo baixado
       *[other] Mais { $count } arquivos sendo baixados
    }
