////////////////////////////////////////////////////////////////////////////
//
//Implementation module: SMT  Memory resident symbol table
//
//Generated file:        SymbTab.cpp
//
//Module identification letters: SMT
//Module identification number:  0320
//
//Repository name:      Test framework support modules
//Repository file name: Z:\TALISMAN\TEST\BSW\UTILIT.BSW
//
//Owning organization:    LES/DI/PUC-Rio
//Project:                Talisman
//List of authors
//   Id      Name
//   avs Arndt von Staa
//
//Software base change control
//    Version  Date         Authors      Description 
//    1     09/02/2007   avs          development begun
//
////////////////////////////////////////////////////////////////////////////

   #include   <stdio.h>
   #include   <string.h>

   #define  _SymbTab_OWN
   #include "SymbTab.hpp"
   #undef   _SymbTab_OWN

//==========================================================================
//----- Encapsulated data declarations -----
//==========================================================================

   #ifdef _DEBUG
      #include   "dynspac.hpp"
   #endif

   #include   "exceptn.hpp"
   #include   "str_smt.inc"


//==========================================================================
//----- Class implementation -----
//==========================================================================

////////////////////////////////////////////////////////////////////////////
// 
// Implementation of class: SMTE Symbol table element value
////////////////////////////////////////////////////////////////////////////

//==========================================================================
//----- Public method implementations -----
//==========================================================================

// Class: SMTE Symbol table element value

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMTE !Symbol table element constructor

   SMTE_SymbolTableElement ::
             SMTE_SymbolTableElement( )
   {

   } // End of function: SMTE !Symbol table element constructor

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMTE !Symbol table element destructor

   SMTE_SymbolTableElement ::
             ~SMTE_SymbolTableElement( )
   {

   } // End of function: SMTE !Symbol table element destructor

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMTE !Verify symbol table element

   bool SMTE_SymbolTableElement ::
             VerifyElement(  const TAL_tpVerifyMode )
   {

      return true ;

   } // End of function: SMTE !Verify symbol table element

//--- End of class: SMTE Symbol table element value


//==========================================================================
//----- Class implementation -----
//==========================================================================

////////////////////////////////////////////////////////////////////////////
// 
// Implementation of class: SMT  Memory resident hashed symbol table
////////////////////////////////////////////////////////////////////////////

//==========================================================================
//----- Public method implementations -----
//==========================================================================

// Class: SMT  Memory resident hashed symbol table

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Symbol table constructor

   SMT_SymbolTable ::
             SMT_SymbolTable( int dimVtHashParm )
   {

      dimVtHash  = dimVtHashParm ;
      HashVector = new SLS_SimpleList[ dimVtHash ] ;

   } // End of function: SMT  !Symbol table constructor

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Symbol table destructor

   SMT_SymbolTable ::
             ~SMT_SymbolTable( )
   {

      delete [] HashVector ;

   } // End of function: SMT  !Symbol table destructor

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Search symbol

   SMTE_SymbolTableElement * SMT_SymbolTable ::
             SearchSymbol( int lenString , char * pString )
   {

      inxHash = Hash( lenString , pString ) ;
      pCollisionList = &( HashVector[ inxHash ] ) ;
      pCollisionList->MoveToFirst( ) ;

      bool HasNext = ( pCollisionList->GetCurrentElement( ) != NULL ) ;

      while ( HasNext )
      {
         SMTE_SymbolTableElement * pCurrentElem =
                   dynamic_cast< SMTE_SymbolTableElement * >
                   ( pCollisionList->GetCurrentElement( ) ) ;
         EXC_ASSERT( pCurrentElem != NULL ) ;
         if ( pCurrentElem->CompareSymbol( lenString , pString ) == TAL_CompareEqual )
         {
            return pCurrentElem ;
         } /* if */
         HasNext = ( pCollisionList->MoveElement( 1 ) == 1 ) ;
      } /* while */

      return NULL ;

   } // End of function: SMT  !Search symbol

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Insert symbol

   SMTE_SymbolTableElement * SMT_SymbolTable ::
             InsertSymbol( SMTE_SymbolTableElement * pSymbol )
   {

      if ( SearchSymbol( pSymbol->GetSymbolString( )->GetLength( ) ,
                         pSymbol->GetSymbolString( )->GetString( )) != NULL )
      {
         return pSymbol ;
      } /* if */

      pCollisionList->InsertAfter( pSymbol ) ;

      return NULL ;

   } // End of function: SMT  !Insert symbol

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Delete symbol

   bool SMT_SymbolTable ::
             DeleteSymbol( int lenString , char * pString )
   {

      if ( SearchSymbol( lenString , pString ) == NULL )
      {
         return false ;
      } /* if */

      pCollisionList->DeleteCurrentElement( ) ;

      return true ;

   } // End of function: SMT  !Delete symbol

////////////////////////////////////////////////////////////////////////////
// 
// Method: SMT  !Verify symbol table structure

   #define   ASSERT_VER( Condition , idMsg )           \
      if ( !( Condition ) )                            \
      {                                                \
         if ( ModeParm == TAL_VerifyLog )              \
         {                                             \
            EXC_LOG( Envelope.pMsg , idMsg ) ;         \
            ErrorCount ++ ;                            \
         } else                                        \
         {                                             \
            return false ;                             \
         }                                             \
      }

   bool SMT_SymbolTable ::
             Verify( const TAL_tpVerifyMode ModeParm )
   {
      // AE: TST_ASSERT( pTabela != NULL ) ;

      unsigned ErrorCount = 0 ;

      // Create symbol table pointer envelope

         struct PointerEnvelope
         {
            MSG_Message * pMsg ;

            PointerEnvelope( ){ pMsg = NULL ; } ;
           ~PointerEnvelope( ){ delete pMsg ; pMsg = NULL ; } ;
         } Envelope ; /* struct */

         if ( ModeParm == TAL_VerifyLog )
         {
            Envelope.pMsg = new MSG_Message( SMT_ErrorRoot ) ;
         } /* if */

      // Verify the head of the table

         ASSERT_VER( HashVector != NULL , SMT_ErrorNoHashVector ) ;
         ASSERT_VER( dimVtHash  >= SMT_minDimVector , SMT_ErrorHashVectorSize ) ;

         if ( HashVector == NULL )
         {
            return false ;
         } /* if */

      // Verify all collision lists

         for ( int inxList = 0 ; inxList < dimVtHash ; inxList ++ ) {

         // Verify this collision list

            SLS_SimpleList * pCurrentList = &( HashVector[ inxList ] ) ;
            ASSERT_VER( pCurrentList->Verify( ModeParm ) , SMT_IncorrectCollisionList ) ;

            pCurrentList->MoveToFirst( ) ;
            bool HasNext = ( pCurrentList->GetCurrentElement( ) != NULL ) ;

            while ( HasNext )
            {

               HasNext = ( pCurrentList->MoveElement( 1 ) == 1 ) ;
            } /* while */

         } // end repetition: Verify all collision lists

      return ErrorCount == 0 ;

   } // End of function: SMT  !Verify symbol table structure

//==========================================================================
//----- Private method implementations -----
//==========================================================================

// Class: SMT  Memory resident hashed symbol table

////////////////////////////////////////////////////////////////////////////
// 
//  Method: SMT  $Compute hash
// 
////////////////////////////////////////////////////////////////////////////

   unsigned SMT_SymbolTable ::
             Hash( int           lenString ,
                   char *        pString    )
   {
      // AE: TST_ASSERT( pSimbolo != NULL ) ;

      unsigned inxHash = 0 ;

      for( int i = 0 ; i < lenString ; i ++ )
      {
         inxHash = ( inxHash << 2 ) + pString[ i ] ;
      } /* for */

      return inxHash % dimVtHash ;

   } // End of function: SMT  $Compute hash

//--- End of class: SMT  Memory resident hashed symbol table

////// End of implementation module: SMT  Memory resident symbol table ////

